/*
 * ACOCore.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: development
 */

#include "ACOCore.h"


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
      METHOD:         Run the chosen II algorithm for the desired number of runs.
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
void ACOCore::Run() {
	m_wriResultsWriter.OpenRFile();
	for(unsigned int i=0; i<m_unRuns;i++){
		m_lfSeed = m_vecSeeds.at(i);
		std::srand ( unsigned ( m_lfSeed ) );
		std::cout << "Run " << i+1 << " - seed " << m_lfSeed << std::endl;
		m_unIterations = 0;
		ACO();

	}
	m_wriResultsWriter.FlushRFile();
}


void ACOCore::ACO() {
	struct timespec sBeginTime;
	struct timespec sEndTime;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sBeginTime);
	do{
		ConstructSolutions();
		UpdateBestSolutions();

		/*Local search*/
		m_cHeuristicCore.SetCurrentSolution(m_cCurrentBestSolution);
		m_cHeuristicCore.ComputeNeighborhood();

		PheromoneUpdate();
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&sEndTime);
		m_lfRunTime = ComputeRunTime(sBeginTime,sEndTime);
		if(m_lfRunTime > m_wriResultsWriter.CurrSamplingTime()){
			m_wriResultsWriter.AddSolutionQuality(m_cCurrentBestSolution.ComputeRelativeSolutionQuality(m_unGlobalOptimum));
			m_wriResultsWriter.NextSamplingTime();
		}
		m_unIterations++;
	}while(m_lfRunTime < m_lfTMax || TerminationCondition());


	std::cout << "Solution found in " << m_lfRunTime << " s" << std::endl;
	std::cout << m_cCurrentBestSolution;
	std::cout << std::endl << std::endl;
	m_wriResultsWriter.AddData(m_lfSeed,m_cCurrentBestSolution.GetTourDuration(),m_cCurrentBestSolution.GetConstraintViolations(),m_lfRunTime);

	m_wriResultsWriter.FlushRTDList();
	m_wriResultsWriter.ResetSolutionQualityList();
	m_wriResultsWriter.RestartSamplingTime();
}


void ACOCore::PheromoneUpdate() {
	PheromoneEvaporation();
	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		PheromoneDeposit(i);
	}
}

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
		if(m_vecAnts[i].GetAntSolution() <= m_vecAnts[i].GetAntSolution()){
			m_unIterationBestSolution = i;
		}
	}

}

void ACOCore::UpdateBestSolutions() {
	int  bestSolution = -1;
	m_unIterationBestSolution = 0;

	for(unsigned int i=0; i<m_vecAnts.size(); i++){
		/* Update best so far solution */
		if(m_vecAnts[i].GetAntSolution() <= m_cCurrentBestSolution){
			bestSolution = i;
		}
		/* Update iteration best solution */
		if(m_vecAnts[i].GetAntSolution() <= m_vecAnts[i].GetAntSolution()){
			m_unIterationBestSolution = i;
		}
	}
	if(bestSolution > 0){
		m_cCurrentBestSolution = m_vecAnts[bestSolution].GetAntSolution();
	}
}

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
			rouletteWheel[i] = pow(m_cPheromoneMatrix.GetElement(previouslyVisitedCity,i),m_lfAlpha) * ComputeHeuristic(previouslyVisitedCity,i);
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
			m_cPheromoneMatrix.SetElement(i,j,SaturatePheromone(m_cPheromoneMatrix(i,j)*(1-m_lfRho)));
			m_cPheromoneMatrix.SetElement(j,i,m_cPheromoneMatrix(i,j));
		}
	}
}

void ACOCore::PheromoneDeposit(unsigned int ant_index) {
	double pheromoneUpdate;
	unsigned int currentCity=0;
	unsigned int nextCity=0;
	CandidateSolution bestSolution;

	/* Probabilistic MAX-MIN update */
	if((float(rand())/RAND_MAX) < m_lfEpsilon){
		/*Update pheromone trails according to global best solution*/
		bestSolution.SetTour(m_cCurrentBestSolution.GetTour());
		bestSolution.SetConstraintViolations(m_cCurrentBestSolution.GetConstraintViolations());
		bestSolution.SetTourDuration(m_cCurrentBestSolution.GetTourDuration());

	}
	else{
		/*Update pheromone trails according to iteration best solution*/
		bestSolution.SetTour(m_vecAnts[m_unIterationBestSolution].GetAntSolution().GetTour());
		bestSolution.SetConstraintViolations(m_vecAnts[m_unIterationBestSolution].GetAntSolution().GetConstraintViolations());
		bestSolution.SetTourDuration(m_vecAnts[m_unIterationBestSolution].GetAntSolution().GetTourDuration());

	}

	pheromoneUpdate = 1.0f/bestSolution.GetTourDuration();

	for(unsigned int i=1; i<bestSolution.GetTour().size()-1; i++){
		currentCity = bestSolution.GetTour().at(i);
		nextCity = bestSolution.GetTour().at(i+1);
		m_cPheromoneMatrix.SetElement(currentCity,nextCity,SaturatePheromone(m_cPheromoneMatrix(currentCity,nextCity)+pheromoneUpdate));
		m_cPheromoneMatrix.SetElement(nextCity,currentCity,m_cPheromoneMatrix(currentCity,nextCity));
	}

}

bool ACOCore::TerminationCondition() {
	if(m_cCurrentBestSolution.GetTourDuration() == m_unGlobalOptimum &&
			m_cCurrentBestSolution.GetConstraintViolations() == 0){
		return true;
	}
	return false;
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
	m_vecAnts[ant_index].GetAntSolution().SetTour(constructedTour);
	ComputeTourLengthAndConstraintsViolations(m_vecAnts[ant_index]);
}

double ACOCore::ComputeHeuristic(unsigned int city_index,unsigned int next_city_index){
	double aComponent = m_lfLambdaA*((m_unAMax-m_vecTimeWindows.at(next_city_index).GetLowerBound())/(m_unAMax-m_unAMin));
	double bComponent = m_lfLambdaB*((m_unBMax-m_vecTimeWindows.at(next_city_index).GetUpperBound())/(m_unBMax-m_unBMin));
	double cComponent = m_lfLambdaC*((m_unCMax-(*m_pcDistanceMatrix)(city_index,next_city_index))/(m_unCMax-m_unCMin));

	return aComponent+bComponent+cComponent;
}

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



double ACOCore::SaturatePheromone(double pheromoneValue) {
	return pheromoneValue < m_lfTauMin ? m_lfTauMin : pheromoneValue > m_lfTauMax ? m_lfTauMax : pheromoneValue;
}


