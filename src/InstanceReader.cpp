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
      File:    InstanceReader.cpp
      Author:  Jacopo De Stefani
      Purpose: Implementation file for the reader module for the instance file
      	  	   containing the distance matrix and the time windows, and the seeds file.
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

#include "InstanceReader.h"

static const unsigned int CITIES_NUMBER_TOKENS=1;
static const unsigned int TIME_WINDOW_TOKENS=2;



bool InstanceReader::OpenFiles() {
	m_ifInputFile.open(m_sInputFileName.c_str());
	if (m_ifInputFile.fail()) {
		return false;
	}
	m_ifSeedsFile.open(m_sSeedsFileName.c_str());
	if (m_ifSeedsFile.fail()) {
		return false;
	}
	return true;
}

bool InstanceReader::ReadInformations() {
	unsigned int readLines = 0;
	std::string currLine;
	std::string tempBuff;
	std::vector<std::string> tokens;
	std::vector<unsigned int> currCityDistances;

	while(!m_ifInputFile.eof()){
		/*Read file line by line and split the string using space as separator*/
		std::getline(m_ifInputFile,currLine);
		std::stringstream splitStream(currLine);

		while(splitStream >> tempBuff){
			tokens.push_back(tempBuff);
		}

		if(tokens.size() > 0){
			/* The first line in the file corresponds to the number of cities */
			if(readLines == 0){
				if(tokens.size() == CITIES_NUMBER_TOKENS){
					unsigned int citiesNumber = 0;
					citiesNumber = atoi(tokens.at(0).c_str());
					m_unCities = citiesNumber;
					m_pcDistanceMatrix = new NumericMatrix<unsigned int>(m_unCities,m_unCities);
				}
				else{
					std::cerr << "Line " << readLines << ": Wrong instance file structure -> Error in cities' number" << std::endl;
					return false;
				}
			}
			/* The following <m_unCities> lines in the file correspond to the rows of the distance matrix */
			else if(0 < readLines && readLines < m_unCities + 1 ){
				if(tokens.size() == m_unCities){
					for(unsigned int i = 0;i < m_unCities;i++){
						currCityDistances.push_back(atoi(tokens.at(i).c_str()));
						m_pcDistanceMatrix->SetElement(readLines-1,i,atoi(tokens.at(i).c_str()));
					}
					currCityDistances.erase(currCityDistances.begin(),currCityDistances.end());
				}
				else{
					std::cerr << "Line " << readLines << ": Wrong instance file structure -> Missing cities' distances" << std::endl;
					return false;
				}
			}
			/* The following <m_unCities> lines in the file correspond to the rows of the distance matrix */
			else if(readLines >= m_unCities + 1){
				if(tokens.size() == TIME_WINDOW_TOKENS){
					TimeWindow currTimeWindow(atoi(tokens.at(0).c_str()),atoi(tokens.at(1).c_str()),readLines-(m_unCities+1));
					m_vecTimeWindows.push_back(currTimeWindow);
				}
				else{
					std::cerr << "Line " << readLines << ": Wrong instance file structure -> Wrong time window structure" << std::endl;
					return false;
				}
			}
			tokens.erase(tokens.begin(),tokens.end());
		}
		readLines++;
	}
	return true;
}

bool InstanceReader::ReadSeeds() {
	unsigned int readLines = 1;
	std::string currLine;
	std::string tempBuff;
	std::vector<std::string> tokens;
	std::vector<unsigned int> currCityDistances;

	while(!m_ifSeedsFile.eof()){
		/*Read file line by line and split the string using space as separator*/
		std::getline(m_ifSeedsFile,currLine);
		std::stringstream splitStream(currLine);

		while(splitStream >> tempBuff){
			tokens.push_back(tempBuff);
		}

		if(tokens.size() > 0){
			if(tokens.size() < 2){
				m_vecSeeds.push_back(atoi(tokens.at(0).c_str()));
			}
			else{
				std::cerr << "Line " << readLines << ": Wrong seeds file structure" << std::endl;
				return false;
			}
			tokens.erase(tokens.begin(),tokens.end());
		}
		readLines++;
	}
	return true;
}

void InstanceReader::PrintDistanceMatrix() {
	std::cout << "DISTANCE MATRIX" << std::endl;
	std::cout << (*m_pcDistanceMatrix) << std::endl;
	/*for(unsigned int i=0; i<m_vecDistanceMatrix.size();i++){
		for(unsigned int j=0; j<m_vecDistanceMatrix.at(i).size();j++){
			std::cout << m_vecDistanceMatrix.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}*/
}



void InstanceReader::PrintTimeWindows() {
	std::cout << "TIME WINDOWS" << std::endl;
	for(unsigned int i=0; i < m_vecTimeWindows.size(); i++){
		std::cout << m_vecTimeWindows.at(i);
	}
}

void InstanceReader::PrintSeeds() {
	std::cout << "SEEDS" << std::endl;
	for(unsigned int i=0; i < m_vecSeeds.size(); i++){
		std::cout << i << ")" << m_vecSeeds.at(i) << std::endl;
	}
}

