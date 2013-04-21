/*
 * ACOCore.h
 *
 *  Created on: Apr 21, 2013
 *      Author: development
 */

#ifndef ACOCORE_H_
#define ACOCORE_H_

#include <climits>
#include <cstdlib>

#include "Ant.h"

class ACOCore {
public:
	ACOCore(const NumericMatrix<unsigned int>* distance_matrix,
			const std::vector<TimeWindow>& vec_time_windows,
			unsigned int cities_number,
			double alpha,
			double beta,
			double rho):
	 m_pcDistanceMatrix(distance_matrix),
	 m_vecTimeWindows(vec_time_windows),
	 m_lfAlpha(alpha),
	 m_lfBeta(beta),
	 m_lfRho(rho),
	 m_unCities(cities_number)
	 {};

	virtual ~ACOCore();
	void PheromoneUpdate();
	void ConstructSolutions();
	unsigned int RouletteWheelSelection(unsigned int,unsigned int);
	void ComputeTourLengthAndConstraintsViolations(Ant&);

private:
	const NumericMatrix<unsigned int>* m_pcDistanceMatrix;
	const std::vector<TimeWindow>& m_vecTimeWindows;
	const std::vector<unsigned int>& m_vecSeeds;
	NumericMatrix<double> m_cPheromoneMatrix;
	std::vector<Ant> m_vecAnts;
	double m_lfAlpha;
	double m_lfBeta;
	double m_lfRho;
	unsigned int m_unCities;


	void PheromoneEvaporation();
	void PheromoneDeposit(unsigned int);
	void ConstructSolutionAnt(unsigned int);

};

#endif /* ACOCORE_H_ */
