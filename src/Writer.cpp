/*
 * Writer.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: deste
 */

#include "Writer.h"

const std::string Writer::PIVOTING_RULE_BEST_IMPROVEMENT="best";
const std::string Writer::PIVOTING_RULE_FIRST_IMPROVEMENT="first";
const std::string Writer::NEIGHBORHOOD_TYPE_INSERT="insert";
const std::string Writer::NEIGHBORHOOD_TYPE_TRANSPOSE="transpose";
const std::string Writer::NEIGHBORHOOD_TYPE_EXCHANGE="exchange";
const std::string Writer::SEPARATOR=".";

void Writer::OpenRFile() {
	// data file
	std::cout << "Filename:" << m_sOutputFileName << std::endl;
	m_ofsRResults.open(m_sOutputFileName.append(".txt").c_str());
	//LOG << "Opening " << m_sStatsFileName << std::endl;
	if (m_ofsRResults.fail()) {
		return;
	}
}

void Writer::OpenTextResults() {
	// data file
	m_ofsTextResults.open(m_sOutputFileName.append(".txt").c_str());
	//LOG << "Opening " << m_sStatsFileName << std::endl;
	if (m_ofsTextResults.fail()) {
		return;
	}
}

void Writer::FlushRFile() {
	m_ofsRResults << "Run\tCV\tCpuTime\tPRPD" << std::endl;
	/*Flush results*/
	for(std::list<SResultsData>::iterator itList=m_listResults.begin(); itList != m_listResults.end(); ++itList){
		m_ofsRResults << (*itList).seed << "\t" << (*itList).constraintViolations << "\t" << (*itList).cpuRunTime << "\t" << (*itList).ComputePRDP(m_unKnownBest) << std::endl;
	}

}

void Writer::FlushTextResults() {
}

void Writer::AddData(double seed,unsigned int best_tour_length, unsigned int constraint_violations, double cpu_time) {
	m_listResults.push_back(SResultsData(seed,best_tour_length,constraint_violations,cpu_time));
}


