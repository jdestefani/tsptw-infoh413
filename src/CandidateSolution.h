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
      File:    CandidateSolution.h
      Author:  Jacopo De Stefani
      Purpose: Header file for the user defined class to
      	  	   model a candidate solution of the problem.
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


#ifndef CANDIDATESOLUTION_H_
#define CANDIDATESOLUTION_H_

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <climits>

#include "CommonDefs.h"

class CandidateSolution {
public:
	CandidateSolution() :
		m_unTourDuration(0),
		m_unConstraintViolations(0),
		m_lfSolutionEvaluation(0.0f){
		m_vecTour.reserve(1);
	}

	CandidateSolution(unsigned int cities) :
			m_unTourDuration(INT_MAX),
			m_unConstraintViolations(cities),
			m_lfSolutionEvaluation(0.0f){
			for(unsigned int i=0; i < cities; i++){
				m_vecTour.push_back(i);
			}
	}

	CandidateSolution(const std::vector<unsigned int>& tour):
		m_vecTour(tour),
		m_unTourDuration(0),
		m_unConstraintViolations(0),
		m_lfSolutionEvaluation(0.0f){
	}

	CandidateSolution(const CandidateSolution& cs):
			m_vecTour(cs.GetTour()),
			m_unTourDuration(cs.GetTourDuration()),
			m_unConstraintViolations(cs.GetConstraintViolations()),
			m_lfSolutionEvaluation(cs.GetTourDuration()){
	}

	virtual ~CandidateSolution();

	inline double GetSolutionEvaluation() const {
		return m_lfSolutionEvaluation;
	}

	inline void SetSolutionEvaluation(double fSolutionEvaluation) {
		m_lfSolutionEvaluation = fSolutionEvaluation;
	}

	inline unsigned int GetConstraintViolations() const {
		return m_unConstraintViolations;
	}

	inline void SetConstraintViolations(unsigned int unConstraintViolations) {
		m_unConstraintViolations = unConstraintViolations;
	}

	inline unsigned int GetTourDuration() const {
		return m_unTourDuration;
	}

	inline void SetTourDuration(unsigned int unTourDuration) {
		m_unTourDuration = unTourDuration;
	}

	const std::vector<unsigned int>& GetTour() const {
		return m_vecTour;
	}

	void SetTour(const std::vector<unsigned int>& vecTour) {
		m_vecTour = vecTour;
		}


	CandidateSolution& operator=(const CandidateSolution& c_cs) {
		//if(!(this->IsTourEqual(c_cs.GetTour()))) {
		SetTour(c_cs.GetTour());
		SetConstraintViolations(c_cs.GetConstraintViolations());
		SetTourDuration(c_cs.GetTourDuration());
		//}
		return *this;
	}


	friend inline std::ostream& operator<<(std::ostream& output, const CandidateSolution& solution){
		output << "Tour: ";
		for(unsigned int i = 0; i < solution.GetTour().size(); i++){
			output << solution.GetTour().at(i) << " ";
		}
		output << std::endl;
		output << "Constraint Violations: " << solution.GetConstraintViolations() << std::endl;
		output << "Total travel time: " << solution.GetTourDuration() << std::endl;
		return output;

	}

	/* Comparison operator among solutions
	 * The current solution is better than another if:
	 * -> Its constraint violations are less than the other solution
	 * -> If constraint violations are equal, then compare tour durations
	 * */
	inline bool operator<(CandidateSolution &other) const{
		return m_unConstraintViolations < other.m_unConstraintViolations;
	}

	inline bool operator<=(CandidateSolution &other) const{
			return m_unConstraintViolations == other.m_unConstraintViolations ? m_unTourDuration < other.m_unTourDuration : m_unConstraintViolations < other.m_unConstraintViolations;
	}

	inline bool operator==(CandidateSolution &other) const{
		return m_unConstraintViolations == other.m_unConstraintViolations;
	}


	bool IsTourEqual(const std::vector<unsigned int>& listCitylist);
	void SwapSolutionComponents(unsigned int,unsigned int);
	void InsertSolutionComponent(unsigned int,unsigned int);
	void ComputeSolutionEvaluation();
	double ComputeRelativeSolutionQuality(double);
	void Reset();
	std::string ToString();



private:
	std::vector<unsigned int> m_vecTour;
	unsigned int m_unTourDuration;
	unsigned int m_unConstraintViolations;
	double m_lfSolutionEvaluation;

};

#endif /* CANDIDATESOLUTION_H_ */
