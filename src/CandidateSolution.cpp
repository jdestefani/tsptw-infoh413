/*
 * CandidateSolution.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#include "CandidateSolution.h"

static const double CONSTRAINT_VIOLATION_PENALTY=10e4;

CandidateSolution::~CandidateSolution() {
	// TODO Auto-generated destructor stub
}

bool CandidateSolution::IsTourEqual(const std::vector<unsigned int>& external_tour) {
	unsigned int i=0;
	unsigned int j=0;

	while(i < m_vecTour.size() && j < external_tour.size())
	{
		if(m_vecTour.at(i) != external_tour.at(j)){
			return false;
		}
		i++;
		j++;
	}
	return true;
}

void CandidateSolution::SwapSolutionComponents(unsigned int firstIndex,unsigned int secondIndex) {
	if(firstIndex != secondIndex){
		unsigned int swapVariable = m_vecTour.at(secondIndex);
		m_vecTour.at(secondIndex) = m_vecTour.at(firstIndex);
		m_vecTour.at(firstIndex) = swapVariable;
	}
}

void CandidateSolution::InsertSolutionComponent(unsigned int city_index, unsigned int insertion_position) {
	if(city_index != insertion_position){
		unsigned int elementToInsert = m_vecTour.at(city_index);
		if(insertion_position > city_index){
			for(unsigned int i=city_index;i<insertion_position;i++){
				m_vecTour.at(i) = m_vecTour.at(i+1);
			}
		}
		else{
			for(unsigned int i=city_index;i>insertion_position;i--){
				m_vecTour.at(i) = m_vecTour.at(i-1);
			}
		}
		m_vecTour.at(insertion_position) = elementToInsert;
	}
}

std::string CandidateSolution::ToString() {
	std::stringstream outputString;
	outputString << "Tour: ";
	for(unsigned int i = 0; i < m_vecTour.size(); i++){
		outputString << m_vecTour.at(i) << " ";
	}
	outputString << std::endl;
	outputString << "Constraint Violations: " << m_unConstraintViolations << std::endl;
	outputString << "Total travel time: " << m_unTourDuration << std::endl;
	return outputString.str();
 }

void CandidateSolution::ComputeSolutionEvaluation() {
	m_fSolutionEvaluation = m_unTourDuration + CONSTRAINT_VIOLATION_PENALTY*m_unConstraintViolations;
}


