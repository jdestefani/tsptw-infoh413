/*
 * InstanceReader.h
 *
 *  Created on: Mar 12, 2013
 *  Author: Jacopo De Stefani
 */

#ifndef INSTANCEREADER_H_
#define INSTANCEREADER_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "TimeWindow.h"

class InstanceReader {
public:
	InstanceReader(std::string file_name) :
		m_sInputFileName(file_name),
		m_unCities(0){}

	InstanceReader() :
		m_sInputFileName(""),
		m_unCities(0){}

	~InstanceReader() {
		m_ifInputFile.close();
	}

	inline const std::string& GetInputFileName() const {
		return m_sInputFileName;
	}

	inline void SetInputFileName(const std::string& sInputFileName) {
		m_sInputFileName = sInputFileName;
	}

	inline unsigned int GetCities() const {
		return m_unCities;
	}

	inline void SetCities(unsigned int unCityNumber) {
		m_unCities = unCityNumber;
	}

	inline const std::vector<TimeWindow>& GetTimeWindows() const {
		return m_vecTimeWindows;
	}

	inline const std::ifstream& GetIfInputFile() const {
		return m_ifInputFile;
	}

	inline const std::vector<std::vector<unsigned int> >& GetDistanceMatrix() const {
		return m_vecDistanceMatrix;
	}

	bool OpenFile();
	bool ReadInformations();
	void PrintDistanceMatrix();
	void PrintTimeWindows();

private:
	std::string m_sInputFileName;
	std::ifstream m_ifInputFile;
	unsigned int m_unCities;
	std::vector<std::vector<unsigned int> > m_vecDistanceMatrix;
	std::vector<TimeWindow> m_vecTimeWindows;

};

#endif /* INSTANCEREADER_H_ */
