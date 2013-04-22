/*
 * ACOCore.h
 *
 *  Created on: Apr 21, 2013
 *      Author: development
 */

#ifndef ACOCORE_H_
#define ACOCORE_H_

#include <climits>
#include <cstdlib>

#include "Ant.h"
#include "Writer.h"
#include "CommonDefs.h"
#include "NumericMatrix.h"

class ACOCore {
public:
	ACOCore(const NumericMatrix<unsigned int>* distance_matrix,
			const std::vector<TimeWindow>& vec_time_windows,
			unsigned int cities_number,
			unsigned int ant_number,
			double alpha,
			double beta,
			double rho,
			const std::vector<unsigned int>& vec_seeds,
			unsigned int runs,
			std::string input_filename,
			unsigned int best_known_solution):
	 m_pcDistanceMatrix(distance_matrix),
	 m_vecTimeWindows(vec_time_windows),
	 m_lfAlpha(alpha),
	 m_lfBeta(beta),
	 m_lfRho(rho),
	 m_unCities(cities_number),
	 m_unAntNumber(ant_number),
	 m_vecSeeds(vec_seeds),
	 m_wriResultsWriter(input_filename,best_known_solution),
	 m_unRuns(runs),
	 m_lfSeed(0.0f),
	 m_lfRunTime(0.0f){
		/*Initialize ants*/
		for(unsigned int i=0; i < ant_number; i++){
			Ant currentAnt;
			m_vecAnts.push_back(currentAnt);
		}

		/*Initialize pheromone matrix*/
		m_cPheromoneMatrix(m_unCities,m_unCities);

		/*Precompute all the heuristic values*/
		m_cHeuristicMatrix(m_unCities,m_unCities);
		for(unsigned int i=0; i < m_unCities; i++){
			for(unsigned int j=0; j < i; j++){
				m_cHeuristicMatrix.SetElement(i,j,pow(1.0f/m_pcDistanceMatrix->GetElement(i,j),m_lfBeta));
				m_cHeuristicMatrix.SetElement(j,i,m_cHeuristicMatrix(i,j));
			}
		}
	};

	virtual ~ACOCore();
	void Run();
	void ACO();
	void PheromoneUpdate();
	void ConstructSolutions();
	unsigned int RouletteWheelSelection(unsigned int,unsigned int);
	void ComputeTourLengthAndConstraintsViolations(Ant&);

private:
	const NumericMatrix<unsigned int>* m_pcDistanceMatrix;
	const std::vector<TimeWindow>& m_vecTimeWindows;
	const std::vector<unsigned int>& m_vecSeeds;
	NumericMatrix<double> m_cPheromoneMatrix;
	NumericMatrix<double> m_cHeuristicMatrix;
	std::vector<Ant> m_vecAnts;
	Writer m_wriResultsWriter;

	double m_lfAlpha;
	double m_lfBeta;
	double m_lfRho;
	double m_lfSeed;
	unsigned int m_unRuns;
	unsigned int m_unCities;
	unsigned int m_unAntNumber;
	CandidateSolution m_cCurrentSolution;
	double m_lfRunTime;

	void PheromoneEvaporation();
	void PheromoneDeposit(unsigned int);
	void ConstructSolutionAnt(unsigned int);

};

#endif /* ACOCORE_H_ */
