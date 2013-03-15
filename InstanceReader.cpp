/*
 * InstanceReader.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: deste
 */

#include "InstanceReader.h"

InstanceReader::InstanceReader(std::string file_name) {
	m_sInputFileName = file_name;
	m_unCities = 0;
}

InstanceReader::InstanceReader() {
	m_sInputFileName = "";
	m_unCities = 0;
}

InstanceReader::~InstanceReader() {
	m_ifInputFile.close();
}

const std::string& InstanceReader::GetInputFileName() const {
	return m_sInputFileName;
}

void InstanceReader::SetInputFileName(const std::string& sInputFileName) {
	m_sInputFileName = sInputFileName;
}

unsigned int InstanceReader::GetCities() const {
	return m_unCities;
}

void InstanceReader::SetCities(unsigned int unCityNumber) {
	m_unCities = unCityNumber;
}

const std::vector<TimeWindow>& InstanceReader::GetVecTimeWindows() const {
	return m_vecTimeWindows;
}

const std::ifstream& InstanceReader::GetIfInputFile() const {
	return m_ifInputFile;
}

const std::vector<std::vector<unsigned int> >& InstanceReader::GetVecDistanceMatrix() const {
	return m_vecDistanceMatrix;
}

bool InstanceReader::OpenFile() {
	m_ifInputFile.open(m_sInputFileName.c_str());
	std::cout << "File opened?" << !m_ifInputFile.fail() << std::endl;
	if (m_ifInputFile.fail()) {
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
		std::getline(m_ifInputFile,currLine);
		std::stringstream splitStream(currLine);

		//std::cout << readLines << ".\t" << currLine << std::endl;
		while(splitStream >> tempBuff){
			tokens.push_back(tempBuff);
		}
		//std::cout << readLines << ".\t" << tokens.size() << std::endl;

		if(tokens.size() > 0){
			if(readLines == 0){
				//Read number of cities
				m_unCities = atoi(tokens.at(0).c_str());
			}
			else if(0 < readLines && readLines < m_unCities + 1 ){
				//Read distances
				for(unsigned int i = 0;i < m_unCities;i++){
					currCityDistances.push_back(atoi(tokens.at(i).c_str()));
				}
				m_vecDistanceMatrix.push_back(currCityDistances);
				currCityDistances.erase(currCityDistances.begin(),currCityDistances.end());
			}
			else if(readLines > m_unCities + 1){
				//Read time window
				TimeWindow currTimeWindow(atoi(tokens.at(0).c_str()),atoi(tokens.at(1).c_str()));
				m_vecTimeWindows.push_back(currTimeWindow);
			}
			tokens.erase(tokens.begin(),tokens.end());
		}
		readLines++;
	}
	return true;
}

void InstanceReader::PrintDistanceMatrix() {
	std::cout << "DISTANCE MATRIX" << std::endl;
	for(unsigned int i=0; i<m_vecDistanceMatrix.size();i++){
		for(unsigned int j=0; j<m_vecDistanceMatrix.at(i).size();j++){
			std::cout << m_vecDistanceMatrix.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}
}

void InstanceReader::PrintTimeWindows() {
	std::cout << "TIME WINDOWS" << std::endl;
	for(unsigned int i=0; i < m_vecTimeWindows.size(); i++){
		std::cout << "[" << m_vecTimeWindows.at(i).GetLowerBound() << "," << m_vecTimeWindows.at(i).GetUpperBound() << "]" << std::endl;
	}
}


