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
	struct timespec currTime;

	m_wriResultsWriter.OpenRFile();
	for(unsigned int i=0; i<m_unRuns;i++){
		clock_gettime(CLOCK_MONOTONIC,&currTime);
		m_fSeed = currTime.tv_sec;
		IterativeImprovement();
	}
	m_wriResultsWriter.FlushRFile();
}

/*
      METHOD:         Implementation of a single run of the algorithm.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
void HeuristicCore::IterativeImprovement() {
	struct timespec sBeginTime;
	struct timespec sEndTime;
	unsigned int iterations = 0;


	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sBeginTime);
	GenerateInitialSolution();
	ComputeNeighborhood();
	std::cout << m_cCurrentSolution;
	while(!IsLocalOptimum()){
		/*1. Select solution from neighborhood*/
		UpdateSolution();
		/*2. Generate neighborhood*/
		ComputeNeighborhood();
		iterations++;
		/*std::cout << "Neighborhood " << iterations << ":" << std::endl;
		for(std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
					std::cout << (*itList) << std::endl;
		}*/
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sEndTime);
	m_fRunTime = (sEndTime.tv_nsec - sBeginTime.tv_nsec)/(10e9);
	std::cout << "Solution found in " << m_fRunTime << "s - (" << iterations << " iterations)" << std::endl;
	std::cout << m_cCurrentSolution;
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
		if((*itList) <= m_cCurrentSolution){
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
void HeuristicCore::ComputeTourLengthAndConstraintsViolations(CandidateSolution& candidateSolution) {
	unsigned int travelTimeAccumulator = 0;
	unsigned int arrivalTimeAccumulator = 0;
	unsigned int i=0;

	/**Reset data*/
	candidateSolution.SetTourDuration(0);
	candidateSolution.SetConstraintViolations(0);

	/**Compute Tour Length*/
	for(; i<candidateSolution.GetTour().size();i++){
		/**Compute Constraint Violations and Waiting Times*/
		/*If the agent arrives in a city before the corresponding time window, delay it until the time window starts*/
		if(arrivalTimeAccumulator < (m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).GetLowerBound())){
			arrivalTimeAccumulator = m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).GetLowerBound();
		}
		/*If the window constraint is not met, add a constraint violation*/
		else if(arrivalTimeAccumulator > (m_vecTimeWindows.at(candidateSolution.GetTour().at(i)).GetUpperBound())){
			candidateSolution.SetConstraintViolations(candidateSolution.GetConstraintViolations()+1);
		}
		if(i < candidateSolution.GetTour().size()-1 ){
			arrivalTimeAccumulator+=m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(i+1));
			travelTimeAccumulator+=m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(i+1));
		}
		else{
			arrivalTimeAccumulator+=m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(0));
			travelTimeAccumulator+=m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(0));
		}
	}
	/*Complete Tour*/
	/*If the agent arrives in a city before the corresponding time window, delay it until the time window starts*/
	if(arrivalTimeAccumulator < (m_vecTimeWindows.at(candidateSolution.GetTour().at(0)).GetLowerBound())){
		arrivalTimeAccumulator = m_vecTimeWindows.at(candidateSolution.GetTour().at(0)).GetLowerBound();
	}
	/*If the window constraint is not met, add a constraint violation*/
	else if(arrivalTimeAccumulator > (m_vecTimeWindows.at(candidateSolution.GetTour().at(0)).GetUpperBound())){
		candidateSolution.SetConstraintViolations(candidateSolution.GetConstraintViolations()+1);
	}

	candidateSolution.SetTourDuration(travelTimeAccumulator);
	candidateSolution.ComputeSolutionEvaluation();

}


void HeuristicCore::GenerateRandomInitialSolution() {
	unsigned int i=0;
	std::vector<unsigned int> currentTour;
	std::srand ( unsigned ( m_fSeed ) );

	for(;i<m_unCities;i++){
		currentTour.push_back(i);
		m_vecTourDistances.push_back(0);
	}
	std::random_shuffle(++currentTour.begin(),currentTour.end());
	std::cout << "Tour length: " << currentTour.size() << std::endl;
	m_cCurrentSolution.SetTour(currentTour);
	ComputeTourLengthAndConstraintsViolations(m_cCurrentSolution);
}

void HeuristicCore::GenerateHeuristicInitialSolution() {
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
	for(unsigned int i=2; i<m_cCurrentSolution.GetTour().size(); i++){
		for(unsigned int j=1; j<i; j++){
			CandidateSolution neighborSolution(m_cCurrentSolution);
			neighborSolution.SwapSolutionComponents(j,i);
			ComputeTourLengthAndConstraintsViolations(neighborSolution);
			//std::cout << "["<< i << "," << j << "]" << neighborSolution << std::endl;
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
			if( i == j || j == i-1 ){
				continue;
			}
				CandidateSolution neighborSolution(m_cCurrentSolution);
				neighborSolution.InsertSolutionComponent(i,j);
				ComputeTourLengthAndConstraintsViolations(neighborSolution);
				//std::cout << "["<< i << "," << j << "]" << neighborSolution << std::endl;
				m_listSolutionNeighborhood.push_back(neighborSolution);
		}
	}
}

void HeuristicCore::UpdateSolutionBestImprovement() {

	std::list<CandidateSolution>::iterator itBest = m_listSolutionNeighborhood.begin();

	for(std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
		if((*itList) < (*itBest)){
			itBest = itList;
		}else if((*itList) <= (*itBest)){
			itBest = itList;
		}
	}

	if(itBest != m_listSolutionNeighborhood.begin()){
		m_cCurrentSolution.SetTour((*itBest).GetTour());
		m_cCurrentSolution.SetTourDuration((*itBest).GetTourDuration());
		m_cCurrentSolution.SetConstraintViolations((*itBest).GetConstraintViolations());
		m_cCurrentSolution.ComputeSolutionEvaluation();
	}
}



void HeuristicCore::UpdateSolutionFirstImprovement() {
	std::list<CandidateSolution>::iterator itFirst = m_listSolutionNeighborhood.end();
	bool isEvaluationImproved = false;

	std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin();
	while(!isEvaluationImproved && itList != m_listSolutionNeighborhood.end()){
		if((*itList) < m_cCurrentSolution){
			isEvaluationImproved = true;
		}
		else{
			if((*itList) <= m_cCurrentSolution && itFirst == m_listSolutionNeighborhood.end()){
				itFirst = itList;
			}
			++itList;
		}
	}

	if(itList != m_listSolutionNeighborhood.end()){
		m_cCurrentSolution.SetTour((*itList).GetTour());
		m_cCurrentSolution.SetTourDuration((*itList).GetTourDuration());
		m_cCurrentSolution.SetConstraintViolations((*itList).GetConstraintViolations());
		m_cCurrentSolution.ComputeSolutionEvaluation();
	}
	else if(itFirst != m_listSolutionNeighborhood.end()){
		m_cCurrentSolution.SetTour((*itFirst).GetTour());
		m_cCurrentSolution.SetTourDuration((*itFirst).GetTourDuration());
		m_cCurrentSolution.SetConstraintViolations((*itFirst).GetConstraintViolations());
		m_cCurrentSolution.ComputeSolutionEvaluation();
	}
}

void HeuristicCore::UpdateListTourDistances() {
	unsigned int i=0;
	for(; i<m_cCurrentSolution.GetTour().size()-1;i++){
		m_vecTourDistances.at(i) = m_vecDistanceMatrix.at(m_cCurrentSolution.GetTour().at(i)).at(m_cCurrentSolution.GetTour().at(i+1));
	}
	m_vecTourDistances.at(i) = m_vecDistanceMatrix.at(m_cCurrentSolution.GetTour().at(i)).at(m_cCurrentSolution.GetTour().at(0));
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

void HeuristicCore::TestFunction() {
	unsigned int iterations = 0;

	std::cout << "Cities: " << m_unCities << std::endl;
	std::cout << "Distance matrix: " << m_vecDistanceMatrix.size() << " x " << m_vecDistanceMatrix.at(80).size() << std::endl;
	std::cout << "Time windows: " << m_vecTimeWindows.size() << std::endl;



		/*GenerateRandomInitialSolution();
		std::cout << m_cCurrentSolution;
		ComputeInsertNeighborhood();
		std::cout << "Neighborhood " << iterations << ":" << std::endl;
		for(std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
			std::cout << (*itList) << std::endl;
		}

		//while(!IsLocalOptimum()){
			1. Select solution from neighborhood
			UpdateSolution();
			2. Generate neighborhood
			ComputeInsertNeighborhood();
			iterations++;
			std::cout << "Neighborhood " << iterations << ":" << std::endl;
			for(std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
						std::cout << (*itList) << std::endl;
			}
		//}
		std::cout << "Solution found in " << m_fRunTime << "s - (" << iterations << " iterations)" << std::endl;
		std::cout << m_cCurrentSolution;
*/

	Run();
	/*ComputeTransposeNeighborhood();
	std::cout << "Generated neighbor size:" << m_listSolutionNeighborhood.size() << std::endl;
	for(std::list<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
			std::cout << (*itList) << std::endl;
		}
	UpdateSolutionBestImprovement();*/
	//UpdateSolutionFirstImprovement();*/
	//IterativeImprovement();
	/*ComputeExchangeNeighborhood();
	std::cout << "Generated neighbor size:" << m_listSolutionNeighborhood.size() << std::endl;
	ComputeInsertNeighborhood();
	std::cout << "Generated neighbor size:" << m_listSolutionNeighborhood.size() << std::endl;*/


}


