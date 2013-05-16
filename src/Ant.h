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
      File:    Ant.h
      Author:  Jacopo De Stefani
      Purpose: Header file for the Ant class used to implement the
       	   	   virtual ant in the SA solver.
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

#ifndef ANT_H_
#define ANT_H_

#include <cassert>
#include <iostream>
#include <vector>
#include "CandidateSolution.h"


class Ant{
public:
	Ant():
	    m_cAntSolution(){
		m_vecVisitedCities.reserve(1);
	}

	Ant(unsigned int cities):
		m_cAntSolution(cities){
		for(unsigned int i=0; i < cities; i++){
			m_vecVisitedCities.push_back(false);
		}
	}

	Ant(const std::vector<unsigned int>& tour):
		m_cAntSolution(tour){
		for(unsigned int i=0; i < tour.size(); i++){
			m_vecVisitedCities.push_back(false);
		}
	}

	~Ant(){}


	const std::vector<unsigned int>& GetVisitedCities() const {
		return m_vecVisitedCities;
	}

	void SetVisitedCity(unsigned int city_index){
		assert(city_index < m_vecVisitedCities.size());
		m_vecVisitedCities[city_index] = true;
	}

	CandidateSolution& GetAntSolution(){
		return m_cAntSolution;
	}

	void SetAntSolution(const CandidateSolution& cAntSolution) {
		m_cAntSolution = cAntSolution;
	}

	void ResetVisitedCities(){
		for(unsigned int i=0; i < m_vecVisitedCities.size() ; i++){
			m_vecVisitedCities[i] = false;
		}
	}

	bool IsVisited(unsigned int i){
		return m_vecVisitedCities[i];
	}


private:
	CandidateSolution m_cAntSolution;
	std::vector<unsigned int> m_vecVisitedCities;
};

#endif /* ANT_H_ */
