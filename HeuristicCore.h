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

#include "TimeWindow.h"
#include "CandidateSolution.h"
#include "Writer.h"

class HeuristicCore {
public:

	enum EInitFunction{
			RANDOM,
			HEURISTIC
		};

	enum ENeighborhoodType{
		EXCHANGE,
		TRANSPOSE,
		INSERT
		};

	enum ESolutionUpdate{
		BEST_IMPROVEMENT,
		FIRST_IMPROVEMENT
	};


	HeuristicCore(std::vector<std::vector<unsigned int> >&,
				  std::vector<TimeWindow>&,unsigned int,
				  EInitFunction,
				  ENeighborhoodType,
				  ESolutionUpdate,
				  double,
				  unsigned int,
				  std::string,
				  unsigned int);

	virtual ~HeuristicCore();

	const CandidateSolution& GetCurrentSolution() const;
	void SetCurrentSolution(const CandidateSolution& cCurrentSolution);
	ENeighborhoodType GetNeighborhoodType() const;
	void SetNeighborhoodType(ENeighborhoodType eNeighborhoodType);
	const std::list<CandidateSolution>& GetListSolutionNeighborhood() const;
	void SetListSolutionNeighborhood(
			const std::list<CandidateSolution>& listSolutionNeighborhood);

	void Run();
	void IterativeImprovement();
	void GenerateInitialSolution();
	void ComputeNeighborhood();
	void UpdateSolution();
	void ComputeTourLengthAndConstraintsViolations(CandidateSolution);
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
	std::vector<std::vector<unsigned int> >& m_vecDistanceMatrix;
	std::vector<TimeWindow>& m_vecTimeWindows;
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
