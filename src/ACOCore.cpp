/*
 * ACOCore.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: development
 */

#include "ACOCore.h"


void ACOCore::PheromoneUpdate() {
	PheromoneEvaporation();
	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		PheromoneDeposit(i);
	}
}

void ACOCore::ConstructSolutions() {
	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		m_vecAnts[i].ResetVisitedCities();
		ConstructSolutionAnt(i);
	}

}

unsigned int ACOCore::RouletteWheelSelection(unsigned int ant_index,unsigned int step) {
	double rouletteWheel[m_unCities];
	double rouletteWheelSum = 0.0f;
	double drawnNumber = 0.0f;
	unsigned int previouslyVisitedCity = m_vecAnts[ant_index].GetTour().at(step-1);
	std::vector<bool> &visitedCities = m_vecAnts[ant_index].GetVisitedCities();
	unsigned int i=0;

	/*Roulette-wheel selection*/
	/*1. Roulette-wheel generation*/
	for(; i<m_unCities;i++){
		if(visitedCities[i]){
			rouletteWheel[i] = 0.0f;
		}
		else{
			rouletteWheel[i] = pow(m_cPheromoneMatrix.GetElement(previouslyVisitedCity,i),m_lfAlpha) * pow(1.0f/m_pcDistanceMatrix->GetElement(previouslyVisitedCity,i),m_lfBeta);
			rouletteWheelSum += rouletteWheel[i];
		}

	}
	/*2. Selection*/
	drawnNumber = (float(rand())/RAND_MAX)*(rouletteWheelSum);

	rouletteWheelSum = 0;
	i=0;
	while(drawnNumber > rouletteWheelSum ){
		rouletteWheelSum += rouletteWheel[i];
		i++;
	}

	return i;

}

void ACOCore::PheromoneEvaporation() {
	for(unsigned int i=0; i<m_cPheromoneMatrix.GetRows(); i++){
		for(unsigned int j=i; j<m_cPheromoneMatrix.GetColumns(); j++){
			m_cPheromoneMatrix.SetElement(i,j,m_cPheromoneMatrix(i,j)*(1-m_lfRho));
			m_cPheromoneMatrix.SetElement(j,i,m_cPheromoneMatrix(i,j));
		}
	}
}

void ACOCore::PheromoneDeposit(unsigned int ant_index) {
	//Define how the pheromone update will be performed
	double pheromoneUpdate = 1.0f/m_vecAnts[ant_index].GetTourDuration();
	unsigned int currentCity=0;
	unsigned int nextCity=0;

	for	(unsigned int i=1; i<m_vecAnts[ant_index].GetTour().size()-1; i++){
		currentCity = m_vecAnts[ant_index].GetTour().at(i);
		nextCity = m_vecAnts[ant_index].GetTour().at(i+1);
		m_cPheromoneMatrix.SetElement(currentCity,nextCity,m_cPheromoneMatrix(currentCity,nextCity)+pheromoneUpdate);
		m_cPheromoneMatrix.SetElement(nextCity,currentCity,m_cPheromoneMatrix(currentCity,nextCity));
	}

}

void ACOCore::ConstructSolutionAnt(unsigned int ant_index) {
	std::vector<unsigned int> constructedTour;
	unsigned int steps=0;
	unsigned int chosenCity=(float(rand())/RAND_MAX)*(m_unCities);


	/*1. Start with the depot*/
	constructedTour.push_back(0);
	/*2. Random pick the first city*/
	constructedTour.push_back(chosenCity);
	m_vecAnts[ant_index].SetVisitedCity(chosenCity);
	/*3. Apply decision rule on all the other cities */
	while(steps < m_unCities-1){
		steps++;
		chosenCity=RouletteWheelSelection(ant_index,steps);
		constructedTour.push_back(chosenCity);
		m_vecAnts[ant_index].SetVisitedCity(chosenCity);
	}
	/*4. Evaluate solution */
	m_vecAnts[ant_index].SetTour(constructedTour);
	ComputeTourLengthAndConstraintsViolations(m_vecAnts[ant_index]);
}

void ACOCore::ComputeTourLengthAndConstraintsViolations(Ant& currentAnt) {
	unsigned int travelTimeAccumulator = 0;
	unsigned int arrivalTimeAccumulator = 0;
	unsigned int i=0;
	unsigned int constraintViolations = 0;
	std::vector<unsigned int> currentTour = currentAnt.GetTour();

	/**Reset data*/
	currentAnt.SetTourDuration(0);
	currentAnt.SetConstraintViolations(0);

	/**Compute Tour Length*/
	for(; i<currentAnt.GetTour().size();i++){
		/**Compute Constraint Violations and Waiting Times*/
		/*If the agent arrives in a city before the corresponding time window, delay it until the time window starts*/
		if(arrivalTimeAccumulator < (m_vecTimeWindows.at(currentTour.at(i)).GetLowerBound())){
			arrivalTimeAccumulator = m_vecTimeWindows.at(currentTour.at(i)).GetLowerBound();
		}
		/*If the window constraint is not met, add a constraint violation*/
		else if(arrivalTimeAccumulator > (m_vecTimeWindows.at(currentTour.at(i)).GetUpperBound())){
			constraintViolations++;
		}

		if(i < currentAnt.GetTour().size()-1 ){
			arrivalTimeAccumulator+=m_pcDistanceMatrix->GetElement(currentTour.at(i),currentTour.at(i+1));
			travelTimeAccumulator+=m_pcDistanceMatrix->GetElement(currentTour.at(i),currentTour.at(i+1));
		}
		else{
			arrivalTimeAccumulator+=m_pcDistanceMatrix->GetElement(currentTour.at(i),currentTour.at(0));
			travelTimeAccumulator+=m_pcDistanceMatrix->GetElement(currentTour.at(i),currentTour.at(0));;
		}
	}
	/*Complete Tour*/
	/*If the agent arrives in a city before the corresponding time window, delay it until the time window starts*/
	if(arrivalTimeAccumulator < (m_vecTimeWindows.at(currentTour.at(0)).GetLowerBound())){
		arrivalTimeAccumulator = m_vecTimeWindows.at(currentTour.at(0)).GetLowerBound();
	}
	/*If the window constraint is not met, add a constraint violation*/
	else if(arrivalTimeAccumulator > (m_vecTimeWindows.at(currentTour.at(0)).GetUpperBound())){
		constraintViolations++;
	}

	currentAnt.SetTourDuration(travelTimeAccumulator);
	currentAnt.SetConstraintViolations(constraintViolations);
	//candidateSolution.ComputeSolutionEvaluation();

}
