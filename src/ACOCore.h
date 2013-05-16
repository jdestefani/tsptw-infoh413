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
      File:    AcoCore.h
      Author:  Jacopo De Stefani
      Purpose: Header file for the core class of the ACO solver
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

#ifndef ACOCORE_H_
#define ACOCORE_H_

#include <limits.h>
#include <stdlib.h>
#include <math.h>

#include "Ant.h"
#include "Writer.h"
#include "CommonDefs.h"
#include "NumericMatrix.h"
#include "HeuristicCore.h"
#include "TimeWindow.h"
#include "CandidateSolution.h"

class ACOCore {
public:
	ACOCore(const NumericMatrix<unsigned int>* distance_matrix,
			const std::vector<TimeWindow>& vec_time_windows,
			unsigned int cities_number,
			unsigned int ant_number,
			double alpha,
			double beta,
			double rho,
			double epsilon,
			double t_max,
			const std::vector<unsigned int>& vec_seeds,
			unsigned int runs,
			std::string input_filename,
			unsigned int best_known_solution):
	 m_pcDistanceMatrix(distance_matrix),
	 m_cPheromoneMatrix(cities_number,cities_number),
	 m_vecTimeWindows(vec_time_windows),
	 m_lfAlpha(alpha),
	 m_lfBeta(beta),
	 m_lfRho(rho),
	 m_lfEpsilon(epsilon),
	 m_unCities(cities_number),
	 m_lfTMax(t_max),
	 m_unAntNumber(ant_number),
	 m_vecSeeds(vec_seeds),
	 m_wriResultsWriter(input_filename,best_known_solution,ANT_COLONY_OPTIMIZATION),
	 m_cHeuristicCore(distance_matrix,
			 	 	  vec_time_windows,
			 	 	  cities_number,
			 	 	  HEURISTIC,
			 	 	  INSERT,
			 	 	  BEST_IMPROVEMENT,
			 	 	  vec_seeds,
			 	 	  runs,
			 	 	  input_filename,
			 	 	  best_known_solution),
	 m_unRuns(runs),
	 m_lfSeed(0.0f),
	 m_lfRunTime(0.0f),
	 m_unIterationBestSolution(0),
	 m_cCurrentBestSolution(cities_number),
	 m_cBestFeasibleSolution(cities_number),
	 m_unGlobalOptimum(best_known_solution),
	 m_unIterations(0){
		/*Initialize ants*/
		for(unsigned int i=0; i < m_unAntNumber; i++){
			Ant currentAnt(m_unCities);
			m_vecAnts.push_back(currentAnt);
		}

		/*Initialize pheromone lower and upper bound*/
		m_lfTauMax = 1.0f/best_known_solution;
		//m_lfTauMax = 1.0f/(m_lfRho*best_known_solution);
		m_lfTauMin = m_lfTauMax/A;


		/*Initialize lambda values*/
		m_lfLambdaA = float(rand())/RAND_MAX;
		m_lfLambdaB = (float(rand())/RAND_MAX)*(1-m_lfLambdaA);
		m_lfLambdaC = (1-m_lfLambdaA-m_lfLambdaB);

		/*m_lfLambdaA = 1.0f/3;
		m_lfLambdaB = 1.0f/3;
		m_lfLambdaC = (1-m_lfLambdaA-m_lfLambdaB);*/

		/*Initialize values required to compute heuristic*/
		InitializeHeuristicValues();
	};

	~ACOCore(){}

	void Run();
	void ACO();

	bool TerminationCondition();
	void ConstructSolutions();
	void UpdateBestSolutions();
	void PheromoneUpdate();
	unsigned int RouletteWheelSelection(unsigned int,unsigned int);
	void ComputeTourLengthAndConstraintsViolations(Ant&);

private:

	static const double A=10.0f;

	const NumericMatrix<unsigned int>* m_pcDistanceMatrix;
	const std::vector<TimeWindow>& m_vecTimeWindows;
	const std::vector<unsigned int>& m_vecSeeds;
	NumericMatrix<double> m_cPheromoneMatrix;
	std::vector<Ant> m_vecAnts;
	Writer m_wriResultsWriter;
	HeuristicCore m_cHeuristicCore;

	double m_lfAlpha;
	double m_lfBeta;
	double m_lfRho;
	double m_lfEpsilon;
	double m_lfTMax;
	double m_lfTauMax;
	double m_lfTauMin;
	double m_lfSeed;
	double m_lfLambdaA;
	double m_lfLambdaB;
	double m_lfLambdaC;
	unsigned int m_unAMax;
	unsigned int m_unAMin;
	unsigned int m_unBMax;
	unsigned int m_unBMin;
	unsigned int m_unCMax;
	unsigned int m_unCMin;
	unsigned int m_unRuns;
	unsigned int m_unCities;
	unsigned int m_unAntNumber;
	CandidateSolution m_cCurrentBestSolution;
	CandidateSolution m_cBestFeasibleSolution;
	unsigned int m_unIterationBestSolution;
	double m_lfRunTime;
	unsigned int m_unIterations;
	unsigned int m_unGlobalOptimum;
	double m_lfTimeOptimum;
	struct timespec m_sBeginTime;
	struct timespec m_sEndTime;

	void InitializeHeuristicValues();
	void PheromoneEvaporation();
	void PheromoneDeposit(unsigned int);
	void ConstructSolutionAnt(unsigned int);
	double SaturatePheromone(double);
	double ComputeHeuristic(unsigned int,unsigned int);
	void SampleSolutionQuality();
	void ResetPheromone();

};

#endif /* ACOCORE_H_ */
