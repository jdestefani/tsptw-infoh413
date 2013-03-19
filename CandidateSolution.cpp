/*
 * CandidateSolution.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#include "CandidateSolution.h"

static const double CONSTRAINT_VIOLATION_PENALTY=10e4;

CandidateSolution::CandidateSolution() {
	m_listCitylist();
	m_unTourLength = 0;
	m_unConstraintViolations = 0;
}

CandidateSolution::~CandidateSolution() {
	// TODO Auto-generated destructor stub
}

const std::list<unsigned int>& CandidateSolution::GetListCitylist() const {
	return m_listCitylist;
}

void CandidateSolution::SetListCitylist(
		const std::list<unsigned int>& listCitylist) {
	m_listCitylist = listCitylist;
}

unsigned int CandidateSolution::GetUnConstraintViolations() const {
	return m_unConstraintViolations;
}

void CandidateSolution::SetUnConstraintViolations(
		unsigned int unConstraintViolations) {
	m_unConstraintViolations = unConstraintViolations;
}

unsigned int CandidateSolution::GetUnTourLength() const {
	return m_unTourLength;
}

void CandidateSolution::SetUnTourLength(unsigned int unTourLength) {
	m_unTourLength = unTourLength;
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
