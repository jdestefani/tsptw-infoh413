/*
 * HeuristicCore.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#include "HeuristicCore.h"


HeuristicCore::HeuristicCore(std::vector<std::vector<unsigned int> >& vec_distance_matrix,std::vector<TimeWindow>& vec_time_windows,
							unsigned int cities_number,EInitFunction init_function,ENeighborhoodType neighborhood_type,ESolutionUpdate solution_update,
							double seed){
	m_vecDistanceMatrix = vec_distance_matrix;
	m_vecTimeWindows = vec_time_windows;
	m_unCities = cities_number;
	m_eInitFunction = init_function;
	m_eNeighborhoodType = neighborhood_type;
	m_eSolutionUpdate = solution_update;
	m_fSeed = seed;
	m_fRunTime = 0;
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


bool HeuristicCore::IsLocalOptimum() {
	std::vector<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin();
	while(itList != m_listSolutionNeighborhood.end()){
		if((*itList).GetTourLength() < m_cCurrentSolution.GetTourLength()){
			return false;
		}
		++itList;
	}

	return true;

}

void HeuristicCore::ComputeTourLengthAndConstraintsViolations(CandidateSolution candidateSolution) {
	/**Compute Tour Length*/
	for(unsigned int i=0; i<candidateSolution.GetTour().size();i++){
		candidateSolution.SetTourLength(candidateSolution.GetTourLength()+m_vecDistanceMatrix.at(candidateSolution.GetTour().at(i)).at(candidateSolution.GetTour().at(i)));
		/**Compute Constraint Violations*/
	}
	/*Complete Tour*/
	candidateSolution.SetTourLength(candidateSolution.GetTourLength()+m_vecDistanceMatrix.at(candidateSolution.GetTour().at(candidateSolution.GetTour().size())).at(candidateSolution.GetTour().at(0)));

}


void HeuristicCore::GenerateRandomInitialSolution() {
	unsigned int i=0;
	std::vector<unsigned int> currentTour;

	for(;i<m_unCities;i++){
		currentTour.push_back(i);
	}
	std::random_shuffle(currentTour.begin(),currentTour.end());
	m_cCurrentSolution.SetTour(currentTour);
}

void HeuristicCore::ComputeTransposeNeighborhood() {
	/*Erase all the neighborhood list*/
	m_listSolutionNeighborhood.erase(m_listSolutionNeighborhood.begin(),m_listSolutionNeighborhood.end());
	/*Possibly consider a differential update of the neighborhood*/

	/*Compute all the possible swaps of an element with his successor*/
	for(unsigned int i=1; i<m_cCurrentSolution.GetTour().size(); i++){
		CandidateSolution neighborSolution(m_cCurrentSolution);
		neighborSolution.SwapSolutionComponents(i-1,i);
		m_listSolutionNeighborhood.push_back(neighborSolution);
	}
}


void HeuristicCore::ComputeExchangeNeighborhood() {
	/*Erase all the neighborhood list*/
	m_listSolutionNeighborhood.erase(m_listSolutionNeighborhood.begin(),m_listSolutionNeighborhood.end());
	/*Possibly consider a differential update of the neighborhood*/

	/*Compute all the possible swaps of an element with his successor*/
	for(unsigned int i=1; i<m_cCurrentSolution.GetTour().size(); i++){
		for(unsigned int j=0; j<i; j++){
			CandidateSolution neighborSolution(m_cCurrentSolution);
			neighborSolution.SwapSolutionComponents(j,i);
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
		for(unsigned int j=0; j<m_cCurrentSolution.GetTour().size(); j++){
			CandidateSolution neighborSolution(m_cCurrentSolution);
			neighborSolution.InsertSolutionComponent(i,j);
			m_listSolutionNeighborhood.push_back(neighborSolution);
		}
	}
}

void HeuristicCore::UpdateSolutionBestImprovement() {

	unsigned int bestTourLength = m_cCurrentSolution.GetTourLength();
	std::vector<CandidateSolution>::iterator itBest = m_listSolutionNeighborhood.begin();

	for(std::vector<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin(); itList != m_listSolutionNeighborhood.end() ; ++itList){
		if((*itList).GetTourLength() < bestTourLength){
			bestTourLength = (*itList).GetTourLength();
			itBest = itList;
		}
	}

	if(itBest != m_listSolutionNeighborhood.begin()){
		m_cCurrentSolution((*itBest).GetTour());
	}
}


void HeuristicCore::UpdateSolutionFirstImprovement() {
	bool isEvaluationImproved = false;

	std::vector<CandidateSolution>::iterator itList = m_listSolutionNeighborhood.begin();
	while(!isEvaluationImproved && itList != m_listSolutionNeighborhood.end()){
		if((*itList).GetTourLength() < m_cCurrentSolution.GetTourLength()){
			isEvaluationImproved = true;
		}
		else{
			++itList;
		}
	}

	if(itList != m_listSolutionNeighborhood.end()){
		m_cCurrentSolution((*itList).GetTour());
	}
}
