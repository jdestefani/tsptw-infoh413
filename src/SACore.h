/*
 * SACore.h
 *
 *  Created on: May 2, 2013
 *      Author: deste
 */

#ifndef SACORE_H_
#define SACORE_H_

#include <limits.h>
#include <stdlib.h>
#include <math.h>

class SACore {
public:
	SACore(const NumericMatrix<unsigned int>* distance_matrix,
			const std::vector<TimeWindow>& vec_time_windows,
				unsigned int cities_number,
				unsigned int ant_number,
				double alpha,
				double t_zero,
				const std::vector<unsigned int>& vec_seeds,
				unsigned int runs,
				std::string input_filename,
				unsigned int best_known_solution):
		 m_pcDistanceMatrix(distance_matrix),
		 m_vecTimeWindows(vec_time_windows),
		 m_unCities(cities_number),
		 m_vecSeeds(vec_seeds),
		 m_wriResultsWriter(input_filename,best_known_solution),
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
		 m_unRuns(runs),
		 m_lfSeed(0.0f),
		 m_lfRunTime(0.0f),
		 m_lfT(t_zero){}

	virtual ~SACore();
	void SA();
	void Run();
	void ProposalMechanism();
	bool AcceptanceCriterion();
	void UpdateTemperature();

private:
		const NumericMatrix<unsigned int>* m_pcDistanceMatrix;
		const std::vector<TimeWindow>& m_vecTimeWindows;
		const std::vector<unsigned int>& m_vecSeeds;
		Writer m_wriResultsWriter;
		HeuristicCore m_cHeuristicCore;

		double m_lfAlpha;
		double m_lfT;
		double m_lfSeed;
		unsigned int m_unRuns;
		unsigned int m_unCities;
		CandidateSolution m_cCurrentSolution;
		CandidateSolution m_cProposedSolution;
		double m_lfRunTime;

};

#endif /* SACORE_H_ */
