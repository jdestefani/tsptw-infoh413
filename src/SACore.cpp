/*
 * SACore.cpp
 *
 *  Created on: May 2, 2013
 *      Author: deste
 */

#include "SACore.h"

SACore::~SACore() {
}

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

void SACore::SA() {
	struct timespec sBeginTime;
	struct timespec sEndTime;
	unsigned int stagnatedIterations = 0;

	//InitTemperature();
	/*Generate initial solution using heuristic*/
	m_cHeuristicCore.SetInitFunction(RANDOM);
	m_cHeuristicCore.GenerateInitialSolution();
	//std::cout << m_cHeuristicCore.GetCurrentSolution();
	m_cCurrentSolution = m_cHeuristicCore.GetCurrentSolutionMutable();

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sBeginTime);
	do{
		ProposalMechanism();

		if(AcceptanceCriterion()){
			m_cCurrentSolution = m_cProposedSolution;
			stagnatedIterations = 0;
		}
		/*If no proposed solution is accepted for a number of iterations corresponding
		 * to the iteration per temperature value, perform a best-improvement local search in the 1-shift
		 * insert neighborhood of the current solution.
		 */

		if(++stagnatedIterations > m_unIPT){
			m_cHeuristicCore.SetCurrentSolution(m_cCurrentSolution);
			m_cHeuristicCore.ComputeNeighborhood();
			m_unIterations+=pow(m_unCities-1,2);
			m_cCurrentSolution = m_cHeuristicCore.GetCurrentSolutionMutable();
			if(m_cCurrentSolution.GetConstraintViolations() == 0
					&& m_cCurrentSolution.GetTourDuration() < m_cBestFeasibleSolution.GetTourDuration()){
				m_cBestFeasibleSolution = m_cCurrentSolution;
				m_lfTimeOptimum = m_lfRunTime;
			}
			stagnatedIterations = 0;

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
			m_wriResultsWriter.AddSolutionQuality(m_cCurrentSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
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

void SACore::ProposalMechanism()
{
	unsigned int i=1+((float(rand())/RAND_MAX)*(m_unCities-1));
	unsigned int j=0;
	std::vector<unsigned int> currentTour = m_cCurrentSolution.GetTour();

	do{
		j=1+((float(rand())/RAND_MAX)*(m_unCities-1));
	}while(j==i);

	//m_cHeuristicCore.SwapTourComponents(currentTour,i,j);
	m_cHeuristicCore.InsertTourComponent(currentTour,i,j);
	m_cProposedSolution.SetTour(currentTour);
	m_cHeuristicCore.ComputeTourLengthAndConstraintsViolations(m_cProposedSolution);
	//std::cout << "Proposed Tour Length:" << m_cProposedSolution.GetTourDuration() << std::endl;
	//std::cout << "Proposed Tour CV:" << m_cProposedSolution.GetConstraintViolations() << std::endl;
}

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

	m_lfT = (accumulator/(i+1)) / -log(m_lfX_zero);
	std::cout << "Initial T:" << m_lfT << std::endl;
}

void SACore::UpdateTemperature()
{
	if((m_unIterations % m_unIPT) == 0){
		m_lfT *= m_lfAlpha;
		m_unTemperatureChanges++;
		m_unTemperatureChangesStationary++;
	}
}

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

