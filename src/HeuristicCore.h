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

      Version: 1.0
      File:    HeuristicCore.h
      Author:  Jacopo De Stefani
      Purpose: Header file for the core class of the heuristic solver
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

#ifndef HEURISTICCORE_H_
#define HEURISTICCORE_H_

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <set>
#include <time.h>
#include <iostream>

#include "TimeWindow.h"
#include "CandidateSolution.h"
#include "NumericMatrix.h"
#include "Writer.h"
#include "CommonDefs.h"

class HeuristicCore {
public:

	HeuristicCore(const NumericMatrix<unsigned int>* p_distance_matrix,
				  const std::vector<TimeWindow>& vec_time_windows,
				  unsigned int cities_number,
				  EInitFunction init_function,
				  ENeighborhoodType neighborhood_type,
				  ESolutionUpdate solution_update,
				  const std::vector<unsigned int>& vec_seeds,
				  unsigned int runs,
				  std::string input_filename,
				  unsigned int best_known_solution):

				  m_pcDistanceMatrix(p_distance_matrix),
				  m_vecTimeWindows(vec_time_windows),
				  m_unCities(cities_number),
				  m_eInitFunction(init_function),
				  m_eNeighborhoodType(neighborhood_type),
				  m_eSolutionUpdate(solution_update),
				  m_eVNDType(NO_VND),
				  m_eNeighborhoodChain(NO_CHAIN),
				  m_vecSeeds(vec_seeds),
				  m_fSeed(0),
				  m_fRunTime(0),
				  m_unRuns(runs),
				  m_wriResultsWriter(input_filename,best_known_solution,neighborhood_type,solution_update),
				  m_bIsLocalOptimum(false),
				  m_bIsImproved(false),
				  m_sBestComponentExchange(0,0,0,0){
	}

	HeuristicCore(const NumericMatrix<unsigned int>* p_distance_matrix,
					  const std::vector<TimeWindow>& vec_time_windows,
					  unsigned int cities_number,
					  EVNDType vnd_type,
					  ENeighborhoodChain neighborhood_chain,
					  const std::vector<unsigned int>& vec_seeds,
					  unsigned int runs,
					  std::string input_filename,
					  unsigned int best_known_solution):

					  m_pcDistanceMatrix(p_distance_matrix),
					  m_vecTimeWindows(vec_time_windows),
					  m_unCities(cities_number),
					  m_eInitFunction(RANDOM),
					  m_eNeighborhoodType(TRANSPOSE),
					  m_eSolutionUpdate(FIRST_IMPROVEMENT),
					  m_eVNDType(vnd_type),
					  m_eNeighborhoodChain(neighborhood_chain),
					  m_vecSeeds(vec_seeds),
					  m_fSeed(0),
					  m_fRunTime(0),
					  m_unRuns(runs),
					  m_wriResultsWriter(input_filename,best_known_solution,vnd_type,neighborhood_chain),
					  m_bIsLocalOptimum(false),
					  m_bIsImproved(false),
					  m_sBestComponentExchange(0,0,0,0){
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

	void RunII();
	void RunVND();
	void IterativeImprovement();
	void VariableNeighborhoodDescent();
	void GenerateNeighborhoodChain();
	void GenerateInitialSolution();
	void ComputeNeighborhood();
	void UpdateSolution();
	void ComputeTourLengthAndConstraintsViolations(CandidateSolution&);
	void ComputeRunTime(struct timespec&,struct timespec&);
	void SwapTourComponents(std::vector<unsigned int>&,unsigned int,unsigned int);
	void InsertTourComponent(std::vector<unsigned int>&,unsigned int,unsigned int);
	bool IsLocalOptimum();

private:

	struct SComponentExchange{
					unsigned int firstElement;
					unsigned int secondElement;
					unsigned int tourDuration;
					unsigned int constraintViolations;

					SComponentExchange(unsigned int first_element, unsigned int second_element, unsigned int tour_duration, unsigned int cv):
						firstElement(first_element),
						secondElement(second_element),
						tourDuration(tour_duration),
						constraintViolations(cv)
						{}

					void Reset(){
						firstElement = 0;
						secondElement = 0;
						tourDuration = 0;
						constraintViolations = 0;
					}

		};

	static const double HEURISTIC_Q = 0.6f;

	EInitFunction m_eInitFunction;
	ENeighborhoodType m_eNeighborhoodType;
	ESolutionUpdate m_eSolutionUpdate;
	EVNDType m_eVNDType;
	ENeighborhoodChain m_eNeighborhoodChain;
	double m_fSeed;
	unsigned int m_unRuns;
	Writer m_wriResultsWriter;

	std::list<CandidateSolution> m_listSolutionNeighborhood;
	CandidateSolution m_cCurrentSolution;
	SComponentExchange m_sBestComponentExchange;
	std::vector<ENeighborhoodType> m_vecNeighborhoodChain;
	bool m_bIsLocalOptimum;
	bool m_bIsImproved;
	unsigned int m_unCities;
	const NumericMatrix<unsigned int>* m_pcDistanceMatrix;
	const std::vector<TimeWindow>& m_vecTimeWindows;
	const std::vector<unsigned int>& m_vecSeeds;
	double m_fRunTime;

	void StandardVariableNeighborhoodDescent();
	void PipedVariableNeighborhoodDescent();
	void GenerateRandomInitialSolution();
	void GenerateHeuristicInitialSolution();
	void ComputeExchangeNeighborhood();
	void ComputeTransposeNeighborhood();
	void ComputeInsertNeighborhood();
	void UpdateSolutionBestImprovement();
	void UpdateSolutionFirstImprovement();
	void UpdateListTourDistances();
	void ComputeTourLengthAndConstraintsViolationsDifferential(unsigned int,unsigned int);
	void UpdateBestExchange(unsigned int,unsigned int,unsigned int,unsigned int);


};

#endif /* HEURISTICCORE_H_ */
