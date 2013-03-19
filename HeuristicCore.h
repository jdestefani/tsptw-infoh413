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

class HeuristicCore {
public:

	enum ENeighborhoodType{
		EXCHANGE,
		TRANSPOSE,
		INSERT
		};
	enum EInitFunction{
		RANDOM,
		HEURISTIC
	};

	HeuristicCore(std::vector<std::vector<unsigned int> >&,std::vector<TimeWindow>&,unsigned int,EInitFunction,ENeighborhoodType);
	virtual ~HeuristicCore();

	const CandidateSolution& GetCurrentSolution() const;
	void SetCurrentSolution(const CandidateSolution& cCurrentSolution);
	ENeighborhoodType GetNeighborhoodType() const;
	void SetNeighborhoodType(ENeighborhoodType eNeighborhoodType);
	const std::list<CandidateSolution>& GetListSolutionNeighborhood() const;
	void SetListSolutionNeighborhood(
			const std::list<CandidateSolution>& listSolutionNeighborhood);

	void GenerateInitialSolution();
	void ComputeNeighborhood();
	void ComputeTourLengthAndConstraintsViolations(CandidateSolution);


private:

	EInitFunction m_eInitFunction;
	ENeighborhoodType m_eNeighborhoodType;
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

};

#endif /* HEURISTICCORE_H_ */
