/*
 * Ant.h
 *
 *  Created on: Apr 21, 2013
 *      Author: development
 */

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
