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
	std::ifstream* getPtrInputFile() const;
	void setPtrInputFile(std::ifstream* ptrInputFile);
	const std::string& GetInputFileName() const;
	void SetInputFileName(const std::string& sInputFileName);
	unsigned int GetCities() const;
	void SetCities(unsigned int unCityNumber);
	const std::vector<TimeWindow>& getVecTimeWindows() const;
	void setVecTimeWindows(const std::vector<TimeWindow>& vecTimeWindows);


private:
	std::string m_sInputFileName;
	std::ifstream* m_ptrInputFile;
	unsigned int m_unCities;
	std::vector<std::vector<unsigned int> > m_vecDistanceMatrix;
	std::vector<TimeWindow> m_vecTimeWindows;

};

#endif /* INSTANCEREADER_H_ */
