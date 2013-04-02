/*
 * HeuristicCore.h
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#ifndef HEURISTICCORE_H_
#define HEURISTICCORE_H_

#include <algorithm>
#include <climits>
#include <set>
#include <time.h>
#include <iostream>

#include "TimeWindow.h"
#include "CandidateSolution.h"
#include "Writer.h"
#include "CommonDefs.h"

class HeuristicCore {
public:

	HeuristicCore(const std::vector<std::vector<unsigned int> >& vec_distance_matrix,
				  const std::vector<TimeWindow>& vec_time_windows,
				  unsigned int cities_number,
				  EInitFunction init_function,
				  ENeighborhoodType neighborhood_type,
				  ESolutionUpdate solution_update,
				  double seed,
				  unsigned int runs,
				  std::string input_filename,
				  unsigned int best_known_solution):

				  m_vecDistanceMatrix(vec_distance_matrix),
				  m_vecTimeWindows(vec_time_windows),
				  m_unCities(cities_number),
				  m_eInitFunction(init_function),
				  m_eNeighborhoodType(neighborhood_type),
				  m_eSolutionUpdate(solution_update),
				  m_fSeed(seed),
				  m_fRunTime(0),
				  m_unRuns(runs),
				  m_wriResultsWriter(input_filename,best_known_solution,neighborhood_type,solution_update){
		//m_vecDistanceMatrix = vec_distance_matrix;
		//m_vecTimeWindows = vec_time_windows;

		//m_wriResultsWriter(input_filename,best_known_solution);
	}

	~HeuristicCore() {
		// TODO Auto-generated destructor stub
	}

	inline const CandidateSolution& GetCurrentSolution() const {
		return m_cCurrentSolution;
	}

	inline void SetCurrentSolution(
			const CandidateSolution& cCurrentSolution) {
		m_cCurrentSolution = cCurrentSolution;
	}

	inline ENeighborhoodType GetNeighborhoodType() const {
		return m_eNeighborhoodType;
	}

	inline void SetNeighborhoodType(ENeighborhoodType eNeighborhoodType) {
		m_eNeighborhoodType = eNeighborhoodType;
	}

	inline const std::list<CandidateSolution>& GetListSolutionNeighborhood() const {
		return m_listSolutionNeighborhood;
	}

	inline void SetListSolutionNeighborhood(const std::list<CandidateSolution>& listSolutionNeighborhood) {
		m_listSolutionNeighborhood = listSolutionNeighborhood;
	}

	void Run();
	void TestFunction();
	void IterativeImprovement();
	void GenerateInitialSolution();
	void ComputeNeighborhood();
	void UpdateSolution();
	void ComputeTourLengthAndConstraintsViolations(CandidateSolution&);
	bool IsLocalOptimum();

private:

	EInitFunction m_eInitFunction;
	ENeighborhoodType m_eNeighborhoodType;
	ESolutionUpdate m_eSolutionUpdate;
	double m_fSeed;
	unsigned int m_unRuns;
	Writer m_wriResultsWriter;

	std::list<CandidateSolution> m_listSolutionNeighborhood;
	CandidateSolution m_cCurrentSolution;
	std::vector<unsigned int> m_vecTourDistances;
	unsigned int m_unCities;
	const std::vector<std::vector<unsigned int> >& m_vecDistanceMatrix;
	const std::vector<TimeWindow>& m_vecTimeWindows;
	double m_fRunTime;

	void GenerateRandomInitialSolution();
	void GenerateHeuristicInitialSolution();
	void ComputeExchangeNeighborhood();
	void ComputeTransposeNeighborhood();
	void ComputeInsertNeighborhood();
	void UpdateSolutionBestImprovement();
	void UpdateSolutionFirstImprovement();
	void UpdateListTourDistances();
	void ComputeTourLengthAndConstraintsViolationsDifferential(unsigned int,unsigned int);

};

#endif /* HEURISTICCORE_H_ */
