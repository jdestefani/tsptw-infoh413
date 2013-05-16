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
      File:    Writer.cpp
      Author:  Jacopo De Stefani
      Purpose: Implementation file for the writer module for the output files
      	  	   to be analysed using R.
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

#include "Writer.h"

const std::string Writer::PIVOTING_RULE_BEST_IMPROVEMENT="best";
const std::string Writer::PIVOTING_RULE_FIRST_IMPROVEMENT="first";
const std::string Writer::NEIGHBORHOOD_TYPE_INSERT="insert";
const std::string Writer::NEIGHBORHOOD_TYPE_TRANSPOSE="transpose";
const std::string Writer::NEIGHBORHOOD_TYPE_EXCHANGE="exchange";
const std::string Writer::NEIGHBORHOOD_CHAIN_TEI="tei";
const std::string Writer::NEIGHBORHOOD_CHAIN_TIE="tie";
const std::string Writer::VND_TYPE_STANDARD="standard";
const std::string Writer::VND_TYPE_PIPED="piped";
const std::string Writer::ACO="ACO";
const std::string Writer::SA="SA";
const std::string Writer::SEPARATOR=".";
const std::string Writer::RESULTAPPENDIX=".txt";
const std::string Writer::RTDAPPENDIX=".rtd";


void Writer::OpenRFile() {
	std::string fileName = m_sOutputFileName;
	std::cout << "\tResults file: " << fileName.append(RESULTAPPENDIX) << std::endl;
	m_ofsRResults.open(fileName.c_str());
	if (m_ofsRResults.fail()) {
		std::cerr << m_sOutputFileName << std::endl;
		return;
	}
}

void Writer::OpenRTDResults() {
	std::string fileName = m_sOutputFileName;
	std::cout << "\tRTDs file: " << fileName.append(RTDAPPENDIX) << std::endl << std::endl;
	m_ofsRTDResults.open(fileName.c_str());

	if (m_ofsRTDResults.fail()) {
		return;
	}
	m_ofsRTDResults << "Seed\t";
	for(std::list<double>::iterator itList=m_listSamplingTimes.begin(); itList != m_listSamplingTimes.end(); ++itList){
			m_ofsRTDResults << (*itList) << "\t";
	}
	m_ofsRTDResults << std::endl;
	m_ofsRTDResults.flush();
}

void Writer::FlushRFile() {
	m_ofsRResults << "Seed\tCV\tCpuTime\tPRPD" << std::endl;
	/*Flush results*/
	for(std::list<SResultsData>::iterator itList=m_listResults.begin(); itList != m_listResults.end(); ++itList){
		m_ofsRResults << (*itList).seed << "\t" << (*itList).constraintViolations << "\t" << (*itList).cpuRunTime << "\t" << (*itList).ComputePRDP(m_unKnownBest) << std::endl;
	}
	m_ofsRResults.close();

}

void Writer::FlushRTDList(double seed) {
	m_ofsRTDResults << seed << "\t";
	/*Flush results*/
	for(std::list<double>::iterator itList=m_listSolutionQuality.begin(); itList != m_listSolutionQuality.end(); ++itList){
		m_ofsRTDResults << (*itList) << "\t";
	}
	m_ofsRTDResults << std::endl;
	m_ofsRTDResults.flush();
}

void Writer::AddData(double seed,unsigned int best_tour_length, unsigned int constraint_violations, double cpu_time) {
	m_listResults.push_back(SResultsData(seed,best_tour_length,constraint_violations,cpu_time));
}

void Writer::AddSolutionQuality(double currentSample) {
	m_listSolutionQuality.push_back(currentSample);
}

void Writer::ResetSolutionQualityList() {
	m_listSolutionQuality.erase(m_listSolutionQuality.begin(),m_listSolutionQuality.end());
}

void Writer::NextSamplingTime() {
	if(m_itNextSamplingTime != m_listSamplingTimes.end()){
		m_itNextSamplingTime++;
	}
}

double Writer::CurrSamplingTime() {
	return m_itNextSamplingTime != m_listSamplingTimes.end() ? (*m_itNextSamplingTime) : DBL_MAX;
}

double Writer::LastSamplingTime() {
	return m_listSamplingTimes.back();
}

void Writer::RestartSamplingTime() {
	m_itNextSamplingTime = m_listSamplingTimes.begin();
}
