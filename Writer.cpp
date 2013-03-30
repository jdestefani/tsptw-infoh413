/*
 * Writer.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: deste
 */

#include "Writer.h"

Writer::Writer(std::string filename,unsigned int known_best) {
	m_sInstanceName = filename.substr(0,filename.find_last_of('.'));
	m_unKnownBest = known_best;

}

Writer::~Writer() {
	if(m_ofsRResults.is_open()){
		m_ofsRResults.close();
	}
	if(m_ofsTextResults.is_open()){
		m_ofsTextResults.close();
	}
}

void Writer::OpenRFile() {
	// data file
	m_ofsRResults.open(m_sInstanceName.append(".r").c_str());
	//LOG << "Opening " << m_sStatsFileName << std::endl;
	if (m_ofsRResults.fail()) {
		return;
	}
}

void Writer::OpenTextResults() {
	// data file
	m_ofsTextResults.open(m_sInstanceName.append(".txt").c_str());
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


