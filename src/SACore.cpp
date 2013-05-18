/*

      TTTTTT   SSSSS  PPPPP	  TTTTTT  W         W
        TT    SS      PP  PP	TT	   W       W
        TT     SSSS   PPPPP		TT      W W W W
        TT        SS  PP		TT		 W W W
        TT    SSSSS   PP		TT		  W W

######################################################
########## Iterative improvement algorithms for ######
########## the TSP problem with Time Windows #########
######################################################

      Version: 1.1
      File:    SACore.cpp
      Author:  Jacopo De Stefani
      Purpose: Implementation file for the core class of the SA solver
      Check:   README and gpl.txt
*/

/***************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    email: jacopo.de.stefani@ulb.ac.be

***************************************************************************/
#include "SACore.h"

/*
      METHOD:         Run the SA algorithm for the desired number of runs.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
void SACore::Run() {
	m_wriResultsWriter.OpenRFile();
	m_wriResultsWriter.OpenRTDResults();
	for(unsigned int i=0; i<m_unRuns;i++){
		m_lfSeed = m_vecSeeds.at(i);
		std::srand( unsigned ( m_lfSeed ) );
		std::cout << "Run " << i+1 << " - seed " << m_lfSeed << std::endl;
		m_unIterations = 0;
		SA();
	}
	m_wriResultsWriter.FlushRFile();
}

/*
      METHOD:         Core method of the SA algorithm.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modify the internal state of the class
*/
void SACore::SA() {
	struct timespec sBeginTime;
	struct timespec sEndTime;

	/*Reset values*/
	m_lfTimeOptimum = 0.0f;
	m_cBestFeasibleSolution.SetConstraintViolations(m_unCities);
	m_cBestFeasibleSolution.SetTourDuration(INT_MAX);

	/*Initialize temperature*/
	m_lfT = m_lfT_zero;

	/*Generate random initial solution*/
	m_cHeuristicCore.SetInitFunction(RANDOM);
	m_cHeuristicCore.GenerateInitialSolution();
	m_cCurrentSolution = m_cHeuristicCore.GetCurrentSolutionMutable();

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sBeginTime);
	do{
		ProposalMechanism();

		if(AcceptanceCriterion()){
			m_cCurrentSolution = m_cProposedSolution;
		}


		UpdateTemperature();

		if(m_cCurrentSolution.GetConstraintViolations() == 0
				&& m_cCurrentSolution.GetTourDuration() < m_cBestFeasibleSolution.GetTourDuration()){
			m_cBestFeasibleSolution = m_cCurrentSolution;
			m_lfTimeOptimum = m_lfRunTime;
		}

		m_unIterations++;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sEndTime);
		m_lfRunTime = ComputeRunTime(sBeginTime,sEndTime);
		if(m_lfRunTime >= m_wriResultsWriter.CurrSamplingTime()){
			//std::cout << m_cCurrentSolution;
			//std::cout << m_wriResultsWriter.CurrSamplingTime() << "@" << m_lfRunTime << ":" << m_cCurrentSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum) << std::endl;
			if(m_lfTimeOptimum > 0.0f){
				m_wriResultsWriter.AddSolutionQuality(m_cBestFeasibleSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
			}
			else{
				m_wriResultsWriter.AddSolutionQuality(m_cCurrentSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
			}
			m_wriResultsWriter.NextSamplingTime();
		}
	}while(!TerminationCondition());


	std::cout << "Solution found in " << m_lfRunTime << " s (" << m_unIterations << " iterations)" << std::endl;
	std::cout << m_cCurrentSolution;
	std::cout << std::endl << std::endl;

	if(m_lfTimeOptimum > 0.0f){
		std::cout << "Best feasible solution found in " << m_lfTimeOptimum << " s" << std::endl;
		std::cout << m_cBestFeasibleSolution;
		std::cout << std::endl << std::endl;
		m_wriResultsWriter.AddData(m_lfSeed,m_cBestFeasibleSolution.GetTourDuration(),m_cBestFeasibleSolution.GetConstraintViolations(),m_lfTimeOptimum);
	}
	else{
		m_wriResultsWriter.AddData(m_lfSeed,m_cCurrentSolution.GetTourDuration(),m_cCurrentSolution.GetConstraintViolations(),m_lfRunTime);
	}


	m_wriResultsWriter.FlushRTDList(m_lfSeed);
	m_wriResultsWriter.ResetSolutionQualityList();
	m_wriResultsWriter.RestartSamplingTime();
}

/*
      METHOD:         Implementation of the solution proposal mechanism.
      INPUT:          None
      OUTPUT:         None
      (SIDE)EFFECTS:  Generate a solution and store it in m_cProposedSolution
*/
void SACore::ProposalMechanism()
{
	unsigned int i=1+((float(rand())/RAND_MAX)*(m_unCities-1));
	unsigned int j=0;
	std::vector<unsigned int> currentTour = m_cCurrentSolution.GetTour();

	/*Regenerate the second index for permutation until it is equal to the first one*/
	do{
		j=1+((float(rand())/RAND_MAX)*(m_unCities-1));
	}while(j==i);

	m_cHeuristicCore.InsertTourComponent(currentTour,i,j);
	m_cProposedSolution.SetTour(currentTour);
	m_cHeuristicCore.ComputeTourLengthAndConstraintsViolations(m_cProposedSolution);

}

/*
      METHOD:         Implementation of the acceptance criterion.
      INPUT:          None
      OUTPUT:         true if the proposed solution is accepted, false otherwise
      (SIDE)EFFECTS:  None
*/
bool SACore::AcceptanceCriterion()
{
	m_cProposedSolution.ComputeSolutionEvaluation();
	m_cCurrentSolution.ComputeSolutionEvaluation();

	if(m_cProposedSolution.GetSolutionEvaluation() <= m_cCurrentSolution.GetSolutionEvaluation()){
		m_unTemperatureChangesStationary = 0;
		return true;
	}

	if(m_cProposedSolution.GetConstraintViolations() == 0){
		m_unTemperatureChangesStationary = 0;
		return true;
	}

	double metropolisValue = exp((m_cCurrentSolution.GetSolutionEvaluation()-m_cProposedSolution.GetSolutionEvaluation())/m_lfT);

	//std::cout << "Metropolis value:" << metropolisValue << std::endl;

	if((float(rand())/RAND_MAX) <= metropolisValue){
		m_unTemperatureChangesStationary = 0;
		return true;
	}

	return false;
}

/*
      METHOD:         Implementation of the temperature initialization process as in "A compressed-annealing heuristic for the traveling salesman
					  problem with time windows",.
      INPUT:          None
      OUTPUT:         true if the proposed solution is accepted, false otherwise
      (SIDE)EFFECTS:  Modifies the value of the temperature in m_lfT
*/
void SACore::InitTemperature() {
	unsigned int i=0;
	double accumulator = 0.0f;

	m_cHeuristicCore.SetInitFunction(RANDOM);
	while(i < R){
		m_cHeuristicCore.GenerateInitialSolution();
		m_cCurrentSolution = m_cHeuristicCore.GetCurrentSolutionMutable();
		ProposalMechanism();
		m_cProposedSolution.ComputeSolutionEvaluation();
		m_cCurrentSolution.ComputeSolutionEvaluation();
		accumulator += abs(m_cCurrentSolution.GetSolutionEvaluation()-m_cProposedSolution.GetSolutionEvaluation());
		i++;
	}

	m_lfT = float(accumulator/(i+1)) / -log(m_lfX_zero);
	std::cout << "Initial T:" << m_lfT << std::endl;
}

/*
      METHOD:         Implementation of the temperature update process.
      INPUT:          None
      OUTPUT:         None
      (SIDE)EFFECTS:  Modifies the value of the temperature in m_lfT
*/
void SACore::UpdateTemperature()
{
	if((m_unIterations % m_unIPT) == 0){
		m_lfT *= m_lfAlpha;
		m_unTemperatureChanges++;
		m_unTemperatureChangesStationary++;
	}
}

/*
      METHOD:         Implementation of the termination condition.
      INPUT:          None
      OUTPUT:         true if the condition is verified, false otherwise.
      (SIDE)EFFECTS:  None
*/
bool SACore::TerminationCondition() {

	if(m_cCurrentSolution.GetTourDuration() == m_unGlobalOptimum &&
			m_cCurrentSolution.GetConstraintViolations() == 0){
		m_cBestFeasibleSolution = m_cCurrentSolution;
		m_lfTimeOptimum = m_lfRunTime;
	}

	if(m_lfRunTime < m_lfTMax){
		if(m_unTemperatureChanges > m_unLowerBoundTemperatureChanges){
			if(m_unTemperatureChangesStationary > CONVERGENCE_THRESHOLD){
				return true;
			}
			return false;
		}
		return false;
	}
	return true;
}

