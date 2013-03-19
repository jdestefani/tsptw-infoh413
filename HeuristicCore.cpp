/*
 * HeuristicCore.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#include "HeuristicCore.h"


HeuristicCore::HeuristicCore(std::vector<std::vector<unsigned int> >& vec_distance_matrix,std::vector<TimeWindow>& vec_time_windows,
							unsigned int cities_number,EInitFunction init_function,ENeighborhoodType neighborhood_type){
	m_vecDistanceMatrix = vec_distance_matrix;
	m_vecTimeWindows = vec_time_windows;
	m_unCities = cities_number;
	m_eInitFunction = init_function;
	m_eNeighborhoodType = neighborhood_type;
}

HeuristicCore::~HeuristicCore() {
	// TODO Auto-generated destructor stub
}

const CandidateSolution& HeuristicCore::GetCurrentSolution() const {
	return m_cCurrentSolution;
}

void HeuristicCore::SetCurrentSolution(
		const CandidateSolution& cCurrentSolution) {
	m_cCurrentSolution = cCurrentSolution;
}

HeuristicCore::ENeighborhoodType HeuristicCore::GetNeighborhoodType() const {
	return m_eNeighborhoodType;
}

void HeuristicCore::SetNeighborhoodType(ENeighborhoodType eNeighborhoodType) {
	m_eNeighborhoodType = eNeighborhoodType;
}

const std::list<CandidateSolution>& HeuristicCore::GetListSolutionNeighborhood() const {
	return m_listSolutionNeighborhood;
}

void HeuristicCore::SetListSolutionNeighborhood(
		const std::list<CandidateSolution>& listSolutionNeighborhood) {
	m_listSolutionNeighborhood = listSolutionNeighborhood;
}

void HeuristicCore::GenerateInitialSolution() {
	switch (m_eNeighborhoodType) {
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

void HeuristicCore::ComputeTourLengthAndConstraintsViolations(
		CandidateSolution candidateSolution) {
	/**Compute Tour Length*/
	std::list<unsigned int>::iterator itNext = candidateSolution.GetCityList().begin();
	++itNext;
	for (std::list<unsigned int>::iterator itList = candidateSolution.GetCityList().begin(); itNext != candidateSolution.GetCityList().end(); ++itList) {
		candidateSolution.SetUnTourLength(candidateSolution.GetUnTourLength()+m_vecDistanceMatrix.at(*itList).at(*itNext));
		/**Compute Constraint Violations*/
	}

}

void HeuristicCore::GenerateRandomInitialSolution() {
	unsigned int i=0;
	std::list<unsigned int> currentTour;

	for(;i<m_unCities;i++){
		currentTour.push_back(i);
	}
	std::random_shuffle(currentTour.begin(),currentTour.end());
	m_cCurrentSolution.SetCityList(currentTour);
}

void HeuristicCore::ComputeExchangeNeighborhood() {
}

void HeuristicCore::ComputeTransposeNeighborhood() {
}

void HeuristicCore::ComputeInsertNeighborhood() {
}
