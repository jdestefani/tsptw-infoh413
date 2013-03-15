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
	InstanceReader();
	InstanceReader(std::string);
	virtual ~InstanceReader();
	bool OpenFile();
	bool ReadInformations();
	void PrintDistanceMatrix();
	void PrintTimeWindows();


	const std::string& GetInputFileName() const;
	void SetInputFileName(const std::string& sInputFileName);
	unsigned int GetCities() const;
	void SetCities(unsigned int unCityNumber);
	const std::vector<TimeWindow>& GetVecTimeWindows() const;
	const std::ifstream& GetIfInputFile() const;
	const std::vector<std::vector<unsigned int> >& GetVecDistanceMatrix() const;

private:
	std::string m_sInputFileName;
	std::ifstream m_ifInputFile;
	unsigned int m_unCities;
	std::vector<std::vector<unsigned int> > m_vecDistanceMatrix;
	std::vector<TimeWindow> m_vecTimeWindows;

};

#endif /* INSTANCEREADER_H_ */
