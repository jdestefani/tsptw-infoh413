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
	// TODO Auto-generated constructor stub

}

Writer::~Writer() {
	// TODO Auto-generated destructor stub
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
	m_ofsRResults << "Run\tConstraintViolations\tDuration\tPRPD" << std::endl;
	/*Flush results*/
}

void Writer::FlushTextResults() {
}

void Writer::AddData(double seed,unsigned int best_tour_length, unsigned int constraint_violations, double cpu_time) {
	m_listSeeds.push_back(seed);
	m_listBestSolutions.push_back(best_tour_length);
	m_listConstraintViolations.push_back(constraint_violations);
	m_listCpuRunTimes.push_back(cpu_time);
	m_listPenalizedRelativePercentageDeviations(ComputePRDP(best_tour_length,constraint_violations));
}

double Writer::ComputePRDP(unsigned int best_tour_length, unsigned int constraint_violations) {
	return 100*(((best_tour_length+constraint_violations)-m_unKnownBest)/m_unKnownBest);
}
