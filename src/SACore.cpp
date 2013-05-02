/*
 * SACore.cpp
 *
 *  Created on: May 2, 2013
 *      Author: deste
 */

#include "SACore.h"

SACore::~SACore() {
	// TODO Auto-generated destructor stub
}

void SACore::Run() {
	m_wriResultsWriter.OpenRFile();
	for(unsigned int i=0; i<m_unRuns;i++){
		m_lfSeed = m_vecSeeds.at(i);
		std::srand ( unsigned ( m_lfSeed ) );
		std::cout << "Run " << i+1 << " - seed " << m_lfSeed << std::endl;
		SA();
	}
	m_wriResultsWriter.FlushRFile();
}

void SACore::SA() {
	/*Generate initial solution using heuristic*/
	m_cHeuristicCore.GenerateInitialSolution();
	m_cCurrentSolution = m_cHeuristicCore.GetCurrentSolution();

	do{
		ProposalMechanism();
		if(AcceptanceCriterion()){
			m_cCurrentSolution = m_cProposedSolution;
		}
		UpdateTemperature();
	}while(!TerminationCondition());

}

void SACore::ProposalMechanism()
{
	unsigned int i=(float(rand())/RAND_MAX)*m_unCities;
	unsigned j=0;
	std::vector<unsigned int>& currentTour = m_cCurrentSolution.GetTour();

	do{
		j=(float(rand())/RAND_MAX)*m_unCities;
	}while(j==i);

	m_cHeuristicCore.InsertTourComponent(currentTour,i,j);
	m_cProposedSolution.SetTour(currentTour);
	m_cHeuristicCore.ComputeTourLengthAndConstraintsViolations(m_cProposedSolution);
}

bool SACore::AcceptanceCriterion()
{

	if(m_cProposedSolution < m_cCurrentSolution){
		return true;
	}

	if(m_cProposedSolution <= m_cCurrentSolution){
		return true;
	}

	double metropolisValue = exp((m_cProposedSolution.GetTourDuration()-m_cCurrentSolution.GetTourDuration())/m_lfT);

	if((float(rand())/RAND_MAX) <= metropolisValue){
		return true;
	}

	return false;
}

void SACore::UpdateTemperature()
{
	m_lfT *= m_lfAlpha;
}
