/*
 * Writer.h
 *
 *  Created on: Mar 22, 2013
 *      Author: deste
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include "CommonDefs.h"

class Writer {
public:

	Writer(std::string filename,
		   unsigned int known_best,
		   ENeighborhoodType neighborhood_type,
		   ESolutionUpdate pivoting_rule):
	m_unKnownBest(known_best),
	m_eNeighborhoodType(neighborhood_type),
	m_ePivotingRule(pivoting_rule){

		m_sOutputFileName = "";

		switch (m_eNeighborhoodType) {
		case EXCHANGE:
			m_sOutputFileName.append(NEIGHBORHOOD_TYPE_EXCHANGE);
			break;
		case TRANSPOSE:
			m_sOutputFileName.append(NEIGHBORHOOD_TYPE_TRANSPOSE);
			break;
		case INSERT:
			m_sOutputFileName.append(NEIGHBORHOOD_TYPE_INSERT);
			break;
		default:
			break;
		}


		m_sOutputFileName.append(SEPARATOR);

		switch (m_ePivotingRule) {
		case BEST_IMPROVEMENT:
			m_sOutputFileName.append(PIVOTING_RULE_BEST_IMPROVEMENT);
			break;
		case FIRST_IMPROVEMENT:
			m_sOutputFileName.append(PIVOTING_RULE_FIRST_IMPROVEMENT);
			break;
		default:
			break;
		}


		m_sOutputFileName.append(SEPARATOR);

		filename = filename.substr(0,filename.find_last_of('.'));
		m_sOutputFileName.append(filename.substr(filename.find_first_of('/')+1,filename.length()));
	}

	~Writer() {
		if(m_ofsRResults.is_open()){
			m_ofsRResults.close();
		}
		if(m_ofsTextResults.is_open()){
			m_ofsTextResults.close();
		}
	}


	void OpenRFile();
	void OpenTextResults();
	void FlushRFile();
	void FlushTextResults();
	void AddData(double,unsigned int,unsigned int,double);


private:

	struct SResultsData{
				double seed;
				unsigned int bestSolution;
				unsigned int constraintViolations;
				double cpuRunTime;

				SResultsData(double seed, unsigned int best_solution, unsigned int cv, double cpu_runtime):
					seed(seed),
					bestSolution(best_solution),
					constraintViolations(cv),
					cpuRunTime(cpu_runtime){

				}

				double ComputePRDP(unsigned int known_best){
					return 100*(((bestSolution+constraintViolations)-known_best)/known_best);
				}
	};

	static const std::string PIVOTING_RULE_BEST_IMPROVEMENT;
	static const std::string PIVOTING_RULE_FIRST_IMPROVEMENT;
	static const std::string NEIGHBORHOOD_TYPE_INSERT;
	static const std::string NEIGHBORHOOD_TYPE_TRANSPOSE;
	static const std::string NEIGHBORHOOD_TYPE_EXCHANGE;
	static const std::string SEPARATOR;

	std::string m_sOutputFileName;
	unsigned int m_unKnownBest;
	std::list<SResultsData> m_listResults;
	std::ofstream m_ofsRResults;
	std::ofstream m_ofsTextResults;
	ESolutionUpdate m_ePivotingRule;
	ENeighborhoodType m_eNeighborhoodType;
};

#endif /* WRITER_H_ */
