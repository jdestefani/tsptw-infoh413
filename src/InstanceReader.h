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
      File:    InstanceReader.h
      Author:  Jacopo De Stefani
      Purpose: Header file for the reader module for the instance file
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


#ifndef INSTANCEREADER_H_
#define INSTANCEREADER_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "TimeWindow.h"
#include "NumericMatrix.h"

class InstanceReader {
public:
	InstanceReader(std::string data_filename,std::string seeds_filename) :
		m_sInputFileName(data_filename),
		m_sSeedsFileName(seeds_filename),
		m_unCities(0),
		m_pcDistanceMatrix(NULL){}

	InstanceReader() :
		m_sInputFileName(""),
		m_unCities(0),
		m_pcDistanceMatrix(NULL){}

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

	const NumericMatrix<unsigned int>* GetPDistanceMatrix() const {
		return m_pcDistanceMatrix;
	}

	inline const std::vector<unsigned int>& GetSeeds() const {
		return m_vecSeeds;
	}

	inline void SetSeeds(const std::vector<unsigned int>& vecSeeds) {
		m_vecSeeds = vecSeeds;
	}

	bool OpenFiles();
	bool ReadInformations();
	bool ReadSeeds();
	void PrintDistanceMatrix();
	void PrintTimeWindows();
	void PrintSeeds();


private:
	std::string m_sInputFileName;
	std::string m_sSeedsFileName;
	std::ifstream m_ifInputFile;
	std::ifstream m_ifSeedsFile;
	unsigned int m_unCities;
	NumericMatrix<unsigned int> *m_pcDistanceMatrix;
	std::vector<TimeWindow> m_vecTimeWindows;
	std::vector<unsigned int> m_vecSeeds;

};

#endif /* INSTANCEREADER_H_ */
