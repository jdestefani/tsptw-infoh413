/*
 * CandidateSolution.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#include "CandidateSolution.h"

static const double CONSTRAINT_VIOLATION_PENALTY=10e4;

CandidateSolution::CandidateSolution() {
	m_vecTour.reserve(1)
	m_unTourLength = 0;
	m_unConstraintViolations = 0;
	m_fSolutionEvaluation = 0.0f;
}

CandidateSolution::CandidateSolution(std::vector<unsigned int>& tour) {
	m_vecTour = tour;
	m_unTourLength = 0;
	m_unConstraintViolations = 0;
	m_fSolutionEvaluation = 0.0f;
}

CandidateSolution::~CandidateSolution() {
	// TODO Auto-generated destructor stub
}

const std::vector<unsigned int>& CandidateSolution::GetTour() const {
	return m_vecTour;
}

void CandidateSolution::SetTour(const std::vector<unsigned int>& vecTour) {
	m_vecTour = vecTour;
}

unsigned int CandidateSolution::GetConstraintViolations() const {
	return m_unConstraintViolations;
}

void CandidateSolution::SetConstraintViolations(
		unsigned int unConstraintViolations) {
	m_unConstraintViolations = unConstraintViolations;
}

unsigned int CandidateSolution::GetTourLength() const {
	return m_unTourLength;
}

void CandidateSolution::SetTourLength(unsigned int unTourLength) {
	m_unTourLength = unTourLength;
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
	unsigned int swapVariable = m_vecTour.at(secondIndex);
	m_vecTour.at(secondIndex) = m_vecTour.at(firstIndex);
	m_vecTour.at(firstIndex) = swapVariable;
}

void CandidateSolution::InsertSolutionComponent(unsigned int unsignedInt) {
}

/*std::ostream& CandidateSolution::operator<<(std::ostream& out,
		const CandidateSolution& solution) {
	out << "Tour:";
	for(std::list<unsigned int>::iterator itList=m_listCitylist.begin(); itList != m_listCitylist.end(); ++itList){
		out << (*itList) << "";
	}
	return out << std::endl;
 }*/

void CandidateSolution::ComputeSolutionEvaluation() {
	m_fSolutionEvaluation = m_unTourLength + CONSTRAINT_VIOLATION_PENALTY*m_unConstraintViolations;
}


