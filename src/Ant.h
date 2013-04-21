/*
 * Ant.h
 *
 *  Created on: Apr 21, 2013
 *      Author: development
 */

#ifndef ANT_H_
#define ANT_H_

#include <vector>
#include "CandidateSolution.h"


class Ant: public CandidateSolution {
public:
	Ant(){
		CandidateSolution::CandidateSolution();
		m_vecVisitedCities.reserve(1);
	}

	Ant(const std::vector<unsigned int>& tour){
		CandidateSolution::CandidateSolution(tour);
		for(unsigned int i=0; i < tour.size(); i++){
			m_vecVisitedCities.push_back(false);
		}
	}

	virtual ~Ant();

	void ResetVisitedCities();
	const std::vector<unsigned int>& GetVisitedCities() const {
		return m_vecVisitedCities;
	}

	void SetVisitedCity(unsigned int city_index){
		m_vecVisitedCities[city_index] = true;
	}



private:
	std::vector<unsigned int> m_vecVisitedCities;
};

#endif /* ANT_H_ */
