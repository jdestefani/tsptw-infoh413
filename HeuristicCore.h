/*
 * HeuristicCore.h
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#ifndef HEURISTICCORE_H_
#define HEURISTICCORE_H_

#include "CandidateSolution.h"
#include <algorithm>
#include <climits>
#include <set>

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


	HeuristicCore(std::vector<std::vector<unsigned int> >&,std::vector<TimeWindow>&,unsigned int,EInitFunction,ENeighborhoodType,ESolutionUpdate);
	virtual ~HeuristicCore();

	const CandidateSolution& GetCurrentSolution() const;
	void SetCurrentSolution(const CandidateSolution& cCurrentSolution);
	ENeighborhoodType GetNeighborhoodType() const;
	void SetNeighborhoodType(ENeighborhoodType eNeighborhoodType);
	const std::list<CandidateSolution>& GetListSolutionNeighborhood() const;
	void SetListSolutionNeighborhood(
			const std::list<CandidateSolution>& listSolutionNeighborhood);

	void IterativeImprovement();
	void GenerateInitialSolution();
	void ComputeNeighborhood();
	void UpdateSolution();
	void ComputeTourLengthAndConstraintsViolations(CandidateSolution);



private:

	EInitFunction m_eInitFunction;
	ENeighborhoodType m_eNeighborhoodType;
	ESolutionUpdate m_eSolutionUpdate;
	double m_fSeed;
	std::list<CandidateSolution> m_listSolutionNeighborhood;
	CandidateSolution m_cCurrentSolution;
	unsigned int m_unCities;
	std::vector<std::vector<unsigned int> >& m_vecDistanceMatrix;
	std::vector<TimeWindow>& m_vecTimeWindows;

	void GenerateRandomInitialSolution();
	void GenerateHeuristicInitialSolution();
	void ComputeExchangeNeighborhood();
	void ComputeTransposeNeighborhood();
	void ComputeInsertNeighborhood();
	void UpdateSolutionBestImprovement();
	void UpdateSolutionFirstImprovement();

};

#endif /* HEURISTICCORE_H_ */
