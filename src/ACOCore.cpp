/*

      TTTTTT   SSSSS  PPPPP	  TTTTTT  W         W
        TT    SS      PP  PP	TT	   W       W
        TT     SSSS   PPPPP		TT      W W W W
        TT        SS  PP		TT		 W W W
        TT    SSSSS   PP		TT		  W W

######################################################
########## Iterative improvement algorithms for ######
########## the TSP problem with Time Windows #########
######################################################

      Version: 1.1
      File:    AcoCore.cpp
      Author:  Jacopo De Stefani
      Purpose: Implementation file for the core class of the ACO solver
      Check:   README and gpl.txt
*/

/***************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    email: jacopo.de.stefani@ulb.ac.be

***************************************************************************/

#include "ACOCore.h"


/*
      METHOD:         Method used to initialize the parameters required to compute the heuristic.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modify the internal state of the class
*/
void ACOCore::InitializeHeuristicValues() {
	unsigned int currA;
	unsigned int currB;

	m_unAMin = m_vecTimeWindows.at(0).GetLowerBound();
	m_unAMax = m_vecTimeWindows.at(0).GetLowerBound();
	m_unBMin = m_vecTimeWindows.at(0).GetUpperBound();
	m_unBMax = m_vecTimeWindows.at(0).GetUpperBound();

	for(unsigned int i=1; i < m_vecTimeWindows.size(); i++){
		currA = m_vecTimeWindows.at(i).GetLowerBound();
		currB = m_vecTimeWindows.at(i).GetUpperBound();

		if(currA < m_unAMin){
			m_unAMin = currA;
		}
		if(currA > m_unAMax){
			m_unAMax = currA;
		}
		if(currB < m_unBMin){
			m_unBMin = currB;
		}
		if(currA > m_unBMax){
			m_unBMax = currB;
		}

	}

	m_unCMin = m_pcDistanceMatrix->GetMin();
	m_unCMax = m_pcDistanceMatrix->GetMax();

}

/*
      METHOD:         Run the ACO algorithm for the desired number of runs.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
void ACOCore::Run() {
	m_wriResultsWriter.OpenRFile();
	m_wriResultsWriter.OpenRTDResults();

	std::cout << "Ranges:" << std::endl;
	std::cout << "\tPheromone range: (" << m_lfTauMin << "," << m_lfTauMax << ")" << std::endl;
	std::cout << "\tOpening time range: (" << m_unAMin << "," << m_unAMax << ") - lambda_a:" << m_lfLambdaA << std::endl;
	std::cout << "\tClosing time range: (" << m_unBMin << "," << m_unBMax << ") - lambda_b:" << m_lfLambdaB << std::endl;
	std::cout << "\tTraveling time: (" << m_unCMin << "," << m_unCMax << ") - lambda_c:" << m_lfLambdaC << std::endl << std::endl;

	for(unsigned int i=0; i<m_unRuns;i++){
		m_lfSeed = m_vecSeeds.at(i);
		std::srand ( unsigned ( m_lfSeed ) );
		std::cout << "Run " << i+1 << " - seed " << m_lfSeed << std::endl;
		m_unIterations = 0;
		ACO();

	}
	m_wriResultsWriter.FlushRFile();
}

/*
      METHOD:         Core method of the ACO algorithm.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modify the internal state of the class
*/
void ACOCore::ACO() {

	/*Reset values*/
	ResetPheromone();
	m_lfTimeOptimum = 0.0f;
	m_cBestFeasibleSolution.SetConstraintViolations(m_unCities);
	m_cBestFeasibleSolution.SetTourDuration(INT_MAX);
	m_cCurrentBestSolution.SetConstraintViolations(m_unCities);
	m_cCurrentBestSolution.SetTourDuration(INT_MAX);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&m_sBeginTime);
	do{
		ConstructSolutions();

		/*Local search*/
		m_cHeuristicCore.SetCurrentSolution(m_cCurrentBestSolution);
		m_cHeuristicCore.ComputeNeighborhood();
		m_cCurrentBestSolution = m_cHeuristicCore.GetCurrentSolutionMutable();

		//UpdateBestSolutions();

		PheromoneUpdate();
		m_unIterations++;

	}while(!TerminationCondition());


	std::cout << "Solution found in " << m_lfRunTime << " s (" << m_unIterations << " iterations)" << std::endl;
	std::cout << m_cCurrentBestSolution;
	std::cout << std::endl << std::endl;

	if(m_lfTimeOptimum > 0.0f){
		std::cout << "Best feasible solution found in " << m_lfTimeOptimum << " s" << std::endl;
		std::cout << m_cBestFeasibleSolution;
		std::cout << std::endl << std::endl;
		m_wriResultsWriter.AddData(m_lfSeed,m_cBestFeasibleSolution.GetTourDuration(),m_cBestFeasibleSolution.GetConstraintViolations(),m_lfTimeOptimum);
	}
	else{
		m_wriResultsWriter.AddData(m_lfSeed,m_cCurrentBestSolution.GetTourDuration(),m_cCurrentBestSolution.GetConstraintViolations(),m_lfRunTime);
	}


	m_wriResultsWriter.FlushRTDList(m_lfSeed);
	m_wriResultsWriter.ResetSolutionQualityList();
	m_wriResultsWriter.RestartSamplingTime();
}

/*
      METHOD:         Method used to sample the solution quality at a certain instant in time and pass it to the writer.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Add a sample to the sample list in the writer
*/
void ACOCore::SampleSolutionQuality() {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&m_sEndTime);
	m_lfRunTime = ComputeRunTime(m_sBeginTime,m_sEndTime);
	if( m_wriResultsWriter.CurrSamplingTime() < m_lfRunTime){ //&& m_lfRunTime <= m_wriResultsWriter.LastSamplingTime()){
		//std::cout << m_wriResultsWriter.CurrSamplingTime() << "@" << m_lfRunTime << ":" << m_cCurrentBestSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum) << std::endl;
		if(m_lfTimeOptimum > 0.0f){
			m_wriResultsWriter.AddSolutionQuality(m_cBestFeasibleSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
		}
		else{
			m_wriResultsWriter.AddSolutionQuality(m_cCurrentBestSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
		}
		m_wriResultsWriter.NextSamplingTime();
	}
}


/*
      METHOD:         Method used to perform the pheromone update (evaporation and deposit).
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modify the internal state of the class
*/
void ACOCore::PheromoneUpdate() {
	PheromoneEvaporation();
	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		PheromoneDeposit(i);
	}
}

/*
      METHOD:         Implementation of the pheromone evaporation process.
      INPUT:          None
      OUTPUT:         None
      (SIDE)EFFECTS:  Modification of the pheromone matrix
*/
void ACOCore::PheromoneEvaporation() {
	for(unsigned int i=0; i<m_cPheromoneMatrix.GetRows(); i++){
		for(unsigned int j=i; j<m_cPheromoneMatrix.GetColumns(); j++){
			m_cPheromoneMatrix.SetElement(i,j,SaturatePheromone(m_cPheromoneMatrix(i,j)*(1-m_lfRho)));
			m_cPheromoneMatrix.SetElement(j,i,m_cPheromoneMatrix(i,j));
		}
	}
}

/*
      METHOD:         Implementation of the pheromone deposit process.
      INPUT:          None
      OUTPUT:         None
      (SIDE)EFFECTS:  Modification the pheromone matrix
*/
void ACOCore::PheromoneDeposit(unsigned int ant_index) {
	double pheromoneUpdate;
	unsigned int currentCity=0;
	unsigned int nextCity=0;
	CandidateSolution bestSolution;

	/* Probabilistic update */
	if((float(rand())/RAND_MAX) < m_lfEpsilon){
		/*Update pheromone trails according to global best solution*/
		bestSolution = m_cCurrentBestSolution;
	}
	else{
		/*Update pheromone trails according to iteration best solution*/
		bestSolution = m_vecAnts[m_unIterationBestSolution].GetAntSolution();
	}

	pheromoneUpdate = 1.0f/bestSolution.GetTourDuration();

	/*Actual pheromone update*/
	for(unsigned int i=0; i<bestSolution.GetTour().size()-1; i++){
		currentCity = bestSolution.GetTour().at(i);
		nextCity = bestSolution.GetTour().at(i+1);
		m_cPheromoneMatrix.SetElement(currentCity,nextCity,SaturatePheromone(m_cPheromoneMatrix(currentCity,nextCity)+pheromoneUpdate));
		m_cPheromoneMatrix.SetElement(nextCity,currentCity,m_cPheromoneMatrix(currentCity,nextCity));
	}

	m_cPheromoneMatrix.SetElement(nextCity,0,SaturatePheromone(m_cPheromoneMatrix(nextCity,0)+pheromoneUpdate));
	m_cPheromoneMatrix.SetElement(0,nextCity,m_cPheromoneMatrix(nextCity,0));


}

/*
      METHOD:         Implementation of the bounds on the pheromone values.
      INPUT:          The pheromone value
      OUTPUT:         The bounded pheromone value
      (SIDE)EFFECTS:  None
*/
double ACOCore::SaturatePheromone(double pheromoneValue) {
	return pheromoneValue < m_lfTauMin ? m_lfTauMin : pheromoneValue > m_lfTauMax ? m_lfTauMax : pheromoneValue;
}

/*
      METHOD:         Implementation of the pheromone reset process.
      INPUT:          None
      OUTPUT:         None
      (SIDE)EFFECTS:  Modification the pheromone matrix
*/
void ACOCore::ResetPheromone() {
	for(unsigned int i=0; i < m_unCities; i++){
		for(unsigned int j=0; j < i; j++){
			m_cPheromoneMatrix.SetElement(i,j,m_lfTauMax);
			m_cPheromoneMatrix.SetElement(j,i,m_cPheromoneMatrix(i,j));
		}
	}
}

/*
      METHOD:         Method used to trigger the solution construction by each ant and to update the best solution.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modify the internal state of the class
*/
void ACOCore::ConstructSolutions() {
	m_unIterationBestSolution = 0;
	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		m_vecAnts[i].ResetVisitedCities();
		ConstructSolutionAnt(i);
		/* Update best so far solution */
		if(m_vecAnts[i].GetAntSolution() <= m_cCurrentBestSolution){
			m_cCurrentBestSolution = m_vecAnts[i].GetAntSolution();
		}
		/* Update iteration best solution */
		if(m_vecAnts[i].GetAntSolution() <= m_vecAnts[m_unIterationBestSolution].GetAntSolution()){
			m_unIterationBestSolution = i;
		}
		SampleSolutionQuality();
	}

	if(m_cCurrentBestSolution.GetConstraintViolations() == 0
			&& m_cCurrentBestSolution.GetTourDuration() < m_cBestFeasibleSolution.GetTourDuration()){
		m_cBestFeasibleSolution = m_cCurrentBestSolution;
		m_lfTimeOptimum = m_lfRunTime;
	}
}

/*
      METHOD:         Implementation of the solution construction process done by each ant.
      INPUT:          None
      OUTPUT:         true if the condition is verified, false otherwise.
      (SIDE)EFFECTS:  None
*/
void ACOCore::ConstructSolutionAnt(unsigned int ant_index) {
	std::vector<unsigned int> constructedTour;
	unsigned int steps=0;
	unsigned int chosenCity=0;

	/*1. Start with the depot*/
	constructedTour.push_back(chosenCity);
	m_vecAnts[ant_index].SetVisitedCity(chosenCity);

	/*2. Random pick the first city*/
	do{
		chosenCity = (float(rand())/RAND_MAX)*m_unCities;
	}while(m_vecAnts[ant_index].IsVisited(chosenCity));
	constructedTour.push_back(chosenCity);
	m_vecAnts[ant_index].SetVisitedCity(chosenCity);

	/*3. Apply decision rule on all the other cities */
	while(steps < m_unCities-2){
		steps++;
		chosenCity=RouletteWheelSelection(ant_index,steps);
		constructedTour.push_back(chosenCity);
		m_vecAnts[ant_index].SetVisitedCity(chosenCity);
	}
	/*4. Evaluate solution */
	m_vecAnts[ant_index].GetAntSolution().SetTour(constructedTour);
	ComputeTourLengthAndConstraintsViolations(m_vecAnts[ant_index]);
	//std::cout << "Ant " << ant_index << ":" <<  m_vecAnts[ant_index].GetAntSolution();
}

/*
      METHOD:         Implementation of the roulette wheel selection process for the probabilistic selection of a new solution component.
      INPUT:          The ant index and the index of the solution component to select
      OUTPUT:         The selected solution component
      (SIDE)EFFECTS:  None
*/
unsigned int ACOCore::RouletteWheelSelection(unsigned int ant_index,unsigned int step) {
	double rouletteWheel[m_unCities];
	double rouletteWheelSum = 0.0f;
	double drawnNumber = 0.0f;
	unsigned int previouslyVisitedCity = m_vecAnts[ant_index].GetAntSolution().GetTour().at(step-1);
	unsigned int i=0;

	/*Roulette-wheel selection*/
	/*1. Roulette-wheel generation*/
	for(; i<m_unCities;i++){
		if(m_vecAnts[ant_index].IsVisited(i)){
			rouletteWheel[i] = 0.0f;
		}
		else{
			//std::cout << "Pheromone:" << m_cPheromoneMatrix(previouslyVisitedCity,i) << "-" << "Heuristic:" << ComputeHeuristic(previouslyVisitedCity,i) << std::endl;
			rouletteWheel[i] = pow(m_cPheromoneMatrix(previouslyVisitedCity,i),m_lfAlpha) * pow(ComputeHeuristic(previouslyVisitedCity,i),m_lfBeta);
			//rouletteWheel[i] = pow(m_cPheromoneMatrix(previouslyVisitedCity,i),m_lfAlpha);
			rouletteWheelSum += rouletteWheel[i];
		}

	}
	/*2. Selection*/
	drawnNumber = (float(rand())/RAND_MAX)*(rouletteWheelSum);

	rouletteWheelSum = 0;
	for(i=0;i<m_unCities;i++){
		rouletteWheelSum += rouletteWheel[i];
		if(drawnNumber < rouletteWheelSum){
			return i;
		}
	}

	return i-1;

}

/*
      METHOD:         Implementation of the computation of the heuristic value for the probabilistic selection.
      INPUT:          None
      OUTPUT:         The heuristic value as a double value.
      (SIDE)EFFECTS:  None
*/
double ACOCore::ComputeHeuristic(unsigned int city_index,unsigned int next_city_index){
	double aComponent = m_lfLambdaA*((m_unAMax-m_vecTimeWindows.at(next_city_index).GetLowerBound())/(m_unAMax-m_unAMin));
	double bComponent = m_lfLambdaB*((m_unBMax-m_vecTimeWindows.at(next_city_index).GetUpperBound())/(m_unBMax-m_unBMin));
	double cComponent = m_lfLambdaC*((m_unCMax-(*m_pcDistanceMatrix)(city_index,next_city_index))/(m_unCMax-m_unCMin));

	return aComponent+bComponent+cComponent;
}

/*
      METHOD:         (Deprecated) Method used to update the current best solution.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modify the internal state of the class
*/
void ACOCore::UpdateBestSolutions() {
	int bestSolution = -1;
	m_unIterationBestSolution = 0;

	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		/* Update best so far solution */
		if(m_vecAnts[i].GetAntSolution() <= m_cCurrentBestSolution){
			bestSolution = i;
		}
		/* Update iteration best solution */
		if(m_vecAnts[i].GetAntSolution() <= m_vecAnts[m_unIterationBestSolution].GetAntSolution()){
			m_unIterationBestSolution = i;
		}
	}

	if(bestSolution >= 0){
		m_cCurrentBestSolution = m_vecAnts[bestSolution].GetAntSolution();
	}
}



/*
      METHOD:         Method to compute the tour duration and number of constraint violations given an ant.
      INPUT:          Reference to the ant.
      OUTPUT:         None.
      (SIDE)EFFECTS:  Updates the tour duration and constraint violation attributes of the ant passed as input.
*/
void ACOCore::ComputeTourLengthAndConstraintsViolations(Ant& currentAnt) {
	unsigned int travelTimeAccumulator = 0;
	unsigned int arrivalTimeAccumulator = 0;
	unsigned int i=0;
	unsigned int constraintViolations = 0;
	std::vector<unsigned int> currentTour = currentAnt.GetAntSolution().GetTour();

	/**Reset data*/
	currentAnt.GetAntSolution().SetTourDuration(0);
	currentAnt.GetAntSolution().SetConstraintViolations(0);

	/**Compute Tour Length*/
	for(; i<currentAnt.GetAntSolution().GetTour().size();i++){
		/**Compute Constraint Violations and Waiting Times*/
		/*If the agent arrives in a city before the corresponding time window, delay it until the time window starts*/
		if(arrivalTimeAccumulator < (m_vecTimeWindows.at(currentTour.at(i)).GetLowerBound())){
			arrivalTimeAccumulator = m_vecTimeWindows.at(currentTour.at(i)).GetLowerBound();
		}
		/*If the window constraint is not met, add a constraint violation*/
		else if(arrivalTimeAccumulator > (m_vecTimeWindows.at(currentTour.at(i)).GetUpperBound())){
			constraintViolations++;
		}

		if(i < currentAnt.GetAntSolution().GetTour().size()-1 ){
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

	currentAnt.GetAntSolution().SetTourDuration(travelTimeAccumulator);
	currentAnt.GetAntSolution().SetConstraintViolations(constraintViolations);
	//candidateSolution.ComputeSolutionEvaluation();

}

/*
      METHOD:         Implementation of the termination condition.
      INPUT:          None
      OUTPUT:         true if the condition is verified, false otherwise.
      (SIDE)EFFECTS:  None
*/
bool ACOCore::TerminationCondition() {

	if(m_cCurrentBestSolution.GetTourDuration() == m_unGlobalOptimum &&
			m_cCurrentBestSolution.GetConstraintViolations() == 0){
		m_cBestFeasibleSolution = m_cCurrentBestSolution;
		m_lfTimeOptimum = m_lfRunTime;
	}

	if(m_lfRunTime < m_lfTMax){
		return false;
	}
	return true;
}



