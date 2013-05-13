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

	InitTemperature();
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
		}
		UpdateTemperature();
		m_unIterations++;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sEndTime);
		m_lfRunTime = ComputeRunTime(sBeginTime,sEndTime);
		if(m_lfRunTime >= m_wriResultsWriter.CurrSamplingTime()){
			m_wriResultsWriter.AddSolutionQuality(m_cCurrentSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
			m_wriResultsWriter.NextSamplingTime();
		}
	}while(m_lfRunTime < m_lfTMax || TerminationCondition());

	std::cout << "Solution found in " << m_lfRunTime << " s" << std::endl;
		std::cout << m_cCurrentSolution;
		std::cout << std::endl << std::endl;
		m_wriResultsWriter.AddData(m_lfSeed,m_cCurrentSolution.GetTourDuration(),m_cCurrentSolution.GetConstraintViolations(),m_lfRunTime);

	m_wriResultsWriter.FlushRTDList(m_lfSeed);
	m_wriResultsWriter.ResetSolutionQualityList();
	m_wriResultsWriter.RestartSamplingTime();
}

void SACore::ProposalMechanism()
{
	unsigned int i=(float(rand())/RAND_MAX)*m_unCities;
	unsigned int j=0;
	std::vector<unsigned int> currentTour = m_cCurrentSolution.GetTour();

	do{
		j=(float(rand())/RAND_MAX)*m_unCities;
	}while(j==i);

	m_cHeuristicCore.InsertTourComponent(currentTour,i,j);
	m_cProposedSolution.SetTour(currentTour);
	m_cHeuristicCore.ComputeTourLengthAndConstraintsViolations(m_cProposedSolution);
	std::cout << "Proposed Tour Length:" << m_cProposedSolution.GetTourDuration() << std::endl;
	std::cout << "Proposed Tour CV:" << m_cProposedSolution.GetConstraintViolations() << std::endl;
}

bool SACore::AcceptanceCriterion()
{
	m_cProposedSolution.ComputeSolutionEvaluation();
	m_cCurrentSolution.ComputeSolutionEvaluation();

	if(m_cProposedSolution.GetSolutionEvaluation() <= m_cCurrentSolution.GetSolutionEvaluation()){
		m_unTemperatureChangesStationary = 0;
		return true;
	}

	double metropolisValue = exp((m_cProposedSolution.GetTourDuration()-m_cCurrentSolution.GetTourDuration())/m_lfT);
	//double metropolisValue = ExpLUT((m_cProposedSolution.GetTourDuration()-m_cCurrentSolution.GetTourDuration())/m_lfT);

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
		accumulator = abs(m_cCurrentSolution.GetTourDuration() - m_cProposedSolution.GetTourDuration());
		i++;
	}

	accumulator /= i+1;
	m_lfT = accumulator / log(1/m_lfX_zero);
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
		return true;
	}

	if(m_unTemperatureChanges > m_unLowerBoundTemperatureChanges){

		if(m_unTemperatureChangesStationary > CONVERGENCE_THRESHOLD){
			return true;
		}
	}
		return false;
}

void SACore::PrecomputeLUT() {
	for(unsigned int i=0 ; i < LUT_UPPERBOUND ; i+= LUT_UPPERBOUND/LUT_RESOLUTION){
		m_vecMetropolisLUT.push_back(exp(-i));
	}
}

double SACore::ExpLUT(double value) {
	unsigned int index = 0;

	if(value <= 0.0f){
		return 1.0f;
	}
	if(value >= LUT_UPPERBOUND){
		return 0.0f;
	}

	index = (unsigned int) (abs(value)/double(LUT_UPPERBOUND/LUT_RESOLUTION));

	return m_vecMetropolisLUT[index];
}
