/*
 * SACore.h
 *
 *  Created on: May 2, 2013
 *      Author: deste
 */

#ifndef SACORE_H_
#define SACORE_H_

#include <limits.h>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <vector>
#include <string>

#include "NumericMatrix.h"
#include "CandidateSolution.h"
#include "CommonDefs.h"
#include "HeuristicCore.h"
#include "Writer.h"

class SACore {
public:
	SACore(const NumericMatrix<unsigned int>* distance_matrix,
			const std::vector<TimeWindow>& vec_time_windows,
				unsigned int cities_number,
				double alpha,
				double T_zero,
				double x_zero,
				unsigned int ipt,
				unsigned int lbtc,
				double t_max,
				const std::vector<unsigned int>& vec_seeds,
				unsigned int runs,
				std::string input_filename,
				unsigned int best_known_solution):
		 m_pcDistanceMatrix(distance_matrix),
		 m_vecTimeWindows(vec_time_windows),
		 m_unCities(cities_number),
		 m_vecSeeds(vec_seeds),
		 m_wriResultsWriter(input_filename,best_known_solution,SIMULATED_ANNEALING),
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
		 m_lfAlpha(alpha),
		 m_lfT(T_zero),
		 m_lfT_zero(T_zero),
		 m_lfX_zero(x_zero),
		 m_unIPT(ipt),
		 m_lfTMax(t_max),
		 m_unLowerBoundTemperatureChanges(lbtc),
		 m_unGlobalOptimum(best_known_solution),
		 m_unRuns(runs),
		 m_lfSeed(0.0f),
		 m_lfRunTime(0.0f),
		 m_unIterations(0),
		 m_unTemperatureChanges(0),
		 m_unTemperatureChangesStationary(0),
		 m_lfTimeOptimum(0.0f),
		 m_cBestFeasibleSolution(cities_number){
	}

	virtual ~SACore();
	void SA();
	void Run();
	void ProposalMechanism();
	bool AcceptanceCriterion();
	void UpdateTemperature();
	bool TerminationCondition();
	void InitTemperature();

private:
		const NumericMatrix<unsigned int>* m_pcDistanceMatrix;
		const std::vector<TimeWindow>& m_vecTimeWindows;
		const std::vector<unsigned int>& m_vecSeeds;
		Writer m_wriResultsWriter;
		HeuristicCore m_cHeuristicCore;

		double m_lfAlpha;
		double m_lfX_zero;
		double m_lfT;
		double m_lfT_zero;
		double m_lfTMax;
		unsigned int m_unIPT;
		double m_lfSeed;
		unsigned int m_unRuns;
		unsigned int m_unCities;
		CandidateSolution m_cCurrentSolution;
		CandidateSolution m_cProposedSolution;
		CandidateSolution m_cBestFeasibleSolution;
		double m_lfRunTime;
		unsigned int m_unGlobalOptimum;
		unsigned int m_unLowerBoundTemperatureChanges;

		unsigned int m_unIterations;
		unsigned int m_unTemperatureChanges;
		unsigned int m_unTemperatureChangesStationary;
		static const unsigned int R=5000;
		static const unsigned int CONVERGENCE_THRESHOLD=75;
		double m_lfTimeOptimum;

};

#endif /* SACORE_H_ */
