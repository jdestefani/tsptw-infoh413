/*
 * HeuristicCore.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#include "HeuristicCore.h"

/*
      METHOD:         Run the chosen algorithm for the desired number of runs.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
void HeuristicCore::Run() {
	for(unsigned int i=0; i<m_unRuns;i++){
		IterativeImprovement();
	}
}

/*
      METHOD:         Implementation of a single run of the algorithm.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
void HeuristicCore::IterativeImprovement() {
	struct timespec* m_sBeginTime;
	struct timespec* m_sEndTime;


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,m_sBeginTime);
	GenerateInitialSolution();
	while(!IsLocalOptimum()){
		/*1. Generate neighborhood*/
		ComputeNeighborhood();
		/*2. Select solution from neighborhood*/
		UpdateSolution();
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,m_sEndTime);
	m_fRunTime = m_sEndTime->tv_sec - m_sBeginTime->tv_sec;
	m_wriResultsWriter.AddData(m_fSeed,m_cCurrentSolution.GetTourDuration(),m_cCurrentSolution.GetConstraintViolations(),m_fRunTime);
}


/*
      METHOD:         Wrapper for the functions used to generate the initial solution.
       	   	   	   	  Launch the correct one according to the value of m_eInitFunction.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Generates the initial solution and store it into the object m_cCurrentSolution
*/
void HeuristicCore::GenerateInitialSolution() {
	switch (m_eInitFunction) {
		case RANDOM:
			GenerateRandomInitialSolution();
			break;
		case HEURISTIC:
			GenerateHeuristicInitialSolution();
			break;
		default:
			break;
	}
}


/*
      METHOD:         Wrapper for the functions used to compute the neighborhood.
       	   	   	   	  Launch the correct one according to the value of m_eNeighborhoodType.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Generates the neighborhood of the current solution and store it into the object m_listSolutionNeighborhood.
*/
void HeuristicCore::ComputeNeighborhood() {
	switch (m_eNeighborhoodType) {
		case EXCHANGE:
			ComputeExchangeNeighborhood();
			break;
		case TRANSPOSE:
			ComputeTransposeNeighborhood();
			break;
		case INSERT:
			ComputeInsertNeighborhood();
			break;
		default:
			break;
	}
}


/*
      METHOD:         Wrapper for the functions used to choose the next solution.
       	   	   	   	  Launch the correct one according to the value of m_eSolutionUpdate.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Explores the neighborhood of the current solution and store it into the object m_cCurrentSolution the chosen solution.
*/
void HeuristicCore::UpdateSolution() {
	switch (m_eSolutionUpdate) {
			case BEST_IMPROVEMENT:
				UpdateSolutionBestImprovement();
				break;
			case FIRST_IMPROVEMENT:
				UpdateSolutionFirstImprovement();
				break;
			default:
				break;
		}
}

/*
      METHOD:         Method used to test the optimality of the solution.
       	   	   	   	  Search in the current neighborhood whether there are better solution that the current one.
      INPUT:          none
      OUTPUT:         true if the solution is the optimal one, false otherwise
      (SIDE)EFFECTS:  none
*/
bool HeuristicCore::IsLocalOptimum() {
	std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin();
	while(itList != m_listSolutionNeighborhood.end()){
		if((*itList).GetTourDuration() < m_cCurrentSolution.GetTourDuration()){
			return false;
		}
		++itList;
	}

	return true;

}


/*
      METHOD:         Method used to compute the tour length and the number of the constraint violations based on the built solutions.
      INPUT:          An object containing the solution for which the values have to be computed
      OUTPUT:         none
      (SIDE)EFFECTS:  Store the computed values in the corresponding attributes of the candidateSolution object passed as parameter.
*/
void HeuristicCore::ComputeTourLengthAndConstraintsViolations(CandidateSolution candidateSolution) {
	unsigned int timeAccumulator = 0;
	/**Compute Tour Length*/
	for(unsigned int i=0; i<candidateSolution.GetTour().size()-1;i++){
		/**Compute Constraint Violations and Waiting Times*/
		/*Delay the current travelling agent until the time window starts*/
		if(timeAccumulator < (m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).GetLowerBound())){
			timeAccumulator = m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).GetLowerBound();
		}
		/*If the window is not meet, add a constraint violation*/
		else if(timeAccumulator > (m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).GetUpperBound())){
			candidateSolution.SetConstraintViolations(candidateSolution.GetConstraintViolations()+1);
		}
		timeAccumulator+=m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(i+1));
	}
	/*Complete Tour*/
	candidateSolution.SetTourDuration(candidateSolution.GetTourDuration()+m_vecDistanceMatrix.at(candidateSolution.GetTour().at(candidateSolution.GetTour().size())).at(candidateSolution.GetTour().at(0)));

}


void HeuristicCore::GenerateRandomInitialSolution() {
	unsigned int i=0;
	std::vector<unsigned int> currentTour;

	for(;i<m_unCities;i++){
		currentTour.push_back(i);
		m_vecTourDistances.push_back(0);
	}
	std::random_shuffle(++currentTour.begin(),currentTour.end());
	m_cCurrentSolution.SetTour(currentTour);
}

void HeuristicCore::ComputeTransposeNeighborhood() {
	/*Erase all the neighborhood list*/
	m_listSolutionNeighborhood.erase(m_listSolutionNeighborhood.begin(),m_listSolutionNeighborhood.end());
	/*Possibly consider a differential update of the neighborhood*/

	/*Compute all the possible swaps of an element with his successor*/
	for(unsigned int i=2; i<m_cCurrentSolution.GetTour().size(); i++){
		CandidateSolution neighborSolution(m_cCurrentSolution);
		neighborSolution.SwapSolutionComponents(i-1,i);
		ComputeTourLengthAndConstraintsViolations(neighborSolution);
		m_listSolutionNeighborhood.push_back(neighborSolution);
	}
}


void HeuristicCore::ComputeExchangeNeighborhood() {
	/*Erase all the neighborhood list*/
	m_listSolutionNeighborhood.erase(m_listSolutionNeighborhood.begin(),m_listSolutionNeighborhood.end());
	/*Possibly consider a differential update of the neighborhood*/

	/*Compute all the possible swaps of an element with his successor*/
	for(unsigned int i=1; i<m_cCurrentSolution.GetTour().size(); i++){
		for(unsigned int j=1; j<i; j++){
			CandidateSolution neighborSolution(m_cCurrentSolution);
			neighborSolution.SwapSolutionComponents(j,i);
			ComputeTourLengthAndConstraintsViolations(neighborSolution);
			m_listSolutionNeighborhood.push_back(neighborSolution);
		}
	}
}

void HeuristicCore::ComputeInsertNeighborhood() {
	/*Erase all the neighborhood list*/
	m_listSolutionNeighborhood.erase(m_listSolutionNeighborhood.begin(),m_listSolutionNeighborhood.end());
	/*Possibly consider a differential update of the neighborhood*/

	/*Compute all the possible swaps of an element with his successor*/
	for(unsigned int i=1; i<m_cCurrentSolution.GetTour().size(); i++){
		for(unsigned int j=1; j<m_cCurrentSolution.GetTour().size(); j++){
			CandidateSolution neighborSolution(m_cCurrentSolution);
			neighborSolution.InsertSolutionComponent(i,j);
			ComputeTourLengthAndConstraintsViolations(neighborSolution);
			m_listSolutionNeighborhood.push_back(neighborSolution);
		}
	}
}

void HeuristicCore::UpdateSolutionBestImprovement() {

	unsigned int bestTourLength = m_cCurrentSolution.GetTourDuration();
	std::list<CandidateSolution>::iterator itBest = m_listSolutionNeighborhood.begin();

	for(std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
		if((*itList).GetTourDuration() < bestTourLength){
			bestTourLength = (*itList).GetTourDuration();
			itBest = itList;
		}
	}

	if(itBest != m_listSolutionNeighborhood.begin()){
		m_cCurrentSolution.SetTour((*itBest).GetTour());
		UpdateListTourDistances();
	}
}



void HeuristicCore::UpdateSolutionFirstImprovement() {
	bool isEvaluationImproved = false;

	std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin();
	while(!isEvaluationImproved && itList != m_listSolutionNeighborhood.end()){
		if((*itList).GetTourDuration() < m_cCurrentSolution.GetTourDuration()){
			isEvaluationImproved = true;
		}
		else{
			++itList;
		}
	}

	if(itList != m_listSolutionNeighborhood.end()){
		m_cCurrentSolution.SetTour((*itList).GetTour());
		UpdateListTourDistances();
	}
}

void HeuristicCore::UpdateListTourDistances() {
	unsigned int i=0;
	for(; i<m_cCurrentSolution.GetTour().size()-1;i++){
		m_vecTourDistances.at(i) = m_vecDistanceMatrix.at(m_cCurrentSolution.GetTour().at(i)).at(m_cCurrentSolution.GetTour().at(i+1));
	}
	m_vecTourDistances.at(i) = m_vecDistanceMatrix.at(m_cCurrentSolution.GetTour().at(i)).at(m_cCurrentSolution.GetTour().at(0));
}

void HeuristicCore::TestFunction() {
	GenerateRandomInitialSolution();
	ComputeTourLengthAndConstraintsViolations(m_cCurrentSolution);
	std::cout << m_cCurrentSolution;
}

/*
      METHOD:         Method used to compute the tour length and the number of the constraint violations based on the built solutions.
      INPUT:          An object containing the solution for which the values have to be computed
      OUTPUT:         none
      (SIDE)EFFECTS:  Store the computed values in the corresponding attributes of the candidateSolution object passed as parameter.
*/
void HeuristicCore::ComputeTourLengthAndConstraintsViolationsDifferential(unsigned int i, unsigned int j) {
	unsigned int currentTourLength = m_cCurrentSolution.GetTourDuration();
	unsigned int distanceAccumulator = 0;
	switch (m_eNeighborhoodType) {
			case EXCHANGE:
				ComputeExchangeNeighborhood();
				break;
			case TRANSPOSE:
				for(unsigned int k=0; k<i; k++){
					distanceAccumulator+= m_vecTourDistances.at(k);
				}
				currentTourLength -= m_vecTourDistances.at(i-1);
				currentTourLength -= m_vecTourDistances.at(j);
				currentTourLength += m_vecDistanceMatrix.at(m_cCurrentSolution.GetTour().at(i)).at(m_cCurrentSolution.GetTour().at(j+1));
				currentTourLength += m_vecDistanceMatrix.at(m_cCurrentSolution.GetTour().at(j)).at(m_cCurrentSolution.GetTour().at(i-1));
				break;
			case INSERT:
				ComputeInsertNeighborhood();
				break;
			default:
				break;
		}

		/**Compute Tour Length
		for(unsigned int i=0; i<candidateSolution.GetTour().size()-1;i++){
			*Compute Constraint Violations
			if(!(m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).InTimeWindowBoundsIncluded(distanceAccumulator))){
				candidateSolution.SetConstraintViolations(candidateSolution.GetConstraintViolations()+1);
			}
			distanceAccumulator+=m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(i+1));
		}
		Complete Tour
		candidateSolution.SetTourLength(candidateSolution.GetTourLength()+m_vecDistanceMatrix.at(candidateSolution.GetTour().at(candidateSolution.GetTour().size())).at(candidateSolution.GetTour().at(0)));
*/
}
