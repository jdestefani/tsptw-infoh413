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
	m_ptrInputFile = NULL;
}

InstanceReader::InstanceReader() {
	m_sInputFileName = "";
	m_unCities = 0;
	m_ptrInputFile = NULL;
}

InstanceReader::~InstanceReader() {
	m_ptrInputFile->close();
}

std::ifstream* InstanceReader::getPtrInputFile() const {
	return m_ptrInputFile;
}

void InstanceReader::setPtrInputFile(std::ifstream* ptrInputFile) {
	m_ptrInputFile = ptrInputFile;
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

const std::vector<TimeWindow>& InstanceReader::getVecTimeWindows() const {
	return m_vecTimeWindows;
}

void InstanceReader::setVecTimeWindows(
		const std::vector<TimeWindow>& vecTimeWindows) {
	m_vecTimeWindows = vecTimeWindows;
}


bool InstanceReader::OpenFile() {
	m_ptrInputFile->open(m_sInputFileName.c_str());

	if (m_ptrInputFile->fail()) {
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

	while(!m_ptrInputFile->eof()){
		std::getline(*m_ptrInputFile,currLine);
		std::stringstream splitStream(currLine);

		while(splitStream >> tempBuff){
			tokens.push_back(tempBuff);
		}

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
		readLines++;
	}
	return true;
}


