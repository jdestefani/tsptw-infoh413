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

void Writer::OpenRFile() {
	// data file
	std::cout << "\tOutput file: " << m_sOutputFileName << std::endl << std::endl;
	m_ofsRResults.open(m_sOutputFileName.append(".txt").c_str());
	if (m_ofsRResults.fail()) {
		std::cerr << m_sOutputFileName << std::endl;
		return;
	}
}

void Writer::OpenTextResults() {
	// data file
	m_ofsTextResults.open(m_sOutputFileName.append(".res").c_str());
	//LOG << "Opening " << m_sStatsFileName << std::endl;
	if (m_ofsTextResults.fail()) {
		return;
	}
}

void Writer::FlushRFile() {
	m_ofsRResults << "Seed\tCV\tCpuTime\tPRPD" << std::endl;
	/*Flush results*/
	for(std::list<SResultsData>::iterator itList=m_listResults.begin(); itList != m_listResults.end(); ++itList){
		m_ofsRResults << (*itList).seed << "\t" << (*itList).constraintViolations << "\t" << (*itList).cpuRunTime << "\t" << (*itList).ComputePRDP(m_unKnownBest) << std::endl;
	}
	m_ofsRResults.close();

}

void Writer::FlushRFileSLS() {
	m_ofsRResults << "Seed\tCV\tPRPD" << std::endl;
	/*Flush results*/
	for(std::list<SResultsData>::iterator itList=m_listResults.begin(); itList != m_listResults.end(); ++itList){
		m_ofsRResults << (*itList).seed << "\t" << (*itList).constraintViolations << "\t" << (*itList).ComputePRDP(m_unKnownBest) << std::endl;
	}
	m_ofsRResults.close();

}


void Writer::AddData(double seed,unsigned int best_tour_length, unsigned int constraint_violations, double cpu_time) {
	m_listResults.push_back(SResultsData(seed,best_tour_length,constraint_violations,cpu_time));
}


