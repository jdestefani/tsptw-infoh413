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
      File:    Writer.h
      Author:  Jacopo De Stefani
      Purpose: Header file for the writer module for the output files
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

#ifndef WRITER_H_
#define WRITER_H_

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include "CommonDefs.h"

class Writer {
public:

	Writer(std::string filename,
		   unsigned int known_best,
		   ENeighborhoodType neighborhood_type,
		   ESolutionUpdate pivoting_rule):
	m_unKnownBest(known_best){

		m_sOutputFileName = "";

		switch (neighborhood_type) {
		case EXCHANGE:
			m_sOutputFileName.append(NEIGHBORHOOD_TYPE_EXCHANGE);
			break;
		case TRANSPOSE:
			m_sOutputFileName.append(NEIGHBORHOOD_TYPE_TRANSPOSE);
			break;
		case INSERT:
			m_sOutputFileName.append(NEIGHBORHOOD_TYPE_INSERT);
			break;
		default:
			break;
		}


		m_sOutputFileName.append(SEPARATOR);

		switch (pivoting_rule) {
		case BEST_IMPROVEMENT:
			m_sOutputFileName.append(PIVOTING_RULE_BEST_IMPROVEMENT);
			break;
		case FIRST_IMPROVEMENT:
			m_sOutputFileName.append(PIVOTING_RULE_FIRST_IMPROVEMENT);
			break;
		default:
			break;
		}


		m_sOutputFileName.append(SEPARATOR);

		filename = filename.substr(0,filename.find_last_of('.'));
		m_sOutputFileName.append(filename.substr(filename.find_last_of('/')+1,filename.length()));
	}

	Writer(std::string filename,
			   unsigned int known_best,
			   EVNDType vnd_type,
			   ENeighborhoodChain neighborhood_chain):
		m_unKnownBest(known_best){

			m_sOutputFileName = "";

			switch (vnd_type) {
			case STANDARD_VND:
				m_sOutputFileName.append(VND_TYPE_STANDARD);
				break;
			case PIPED_VND:
				m_sOutputFileName.append(VND_TYPE_PIPED);
				break;
			default:
				break;
			}


			m_sOutputFileName.append(SEPARATOR);

			switch (neighborhood_chain) {
			case TRANSPOSE_EXCHANGE_INSERT:
				m_sOutputFileName.append(NEIGHBORHOOD_CHAIN_TEI);
				break;
			case TRANSPOSE_INSERT_EXCHANGE:
				m_sOutputFileName.append(NEIGHBORHOOD_CHAIN_TIE);
				break;
			default:
				break;
			}


			m_sOutputFileName.append(SEPARATOR);

			filename = filename.substr(0,filename.find_last_of('.'));
			m_sOutputFileName.append(filename.substr(filename.find_last_of('/')+1,filename.length()));
		}

	~Writer() {
		if(m_ofsRResults.is_open()){
			m_ofsRResults.close();
		}
		if(m_ofsTextResults.is_open()){
			m_ofsTextResults.close();
		}
	}


	void OpenRFile();
	void OpenTextResults();
	void FlushRFile();
	void AddData(double,unsigned int,unsigned int,double);



private:

	struct SResultsData{
				double seed;
				unsigned int bestSolution;
				unsigned int constraintViolations;
				double cpuRunTime;
				static const double CONSTRAINT_VIOLATION_PENALTY=10e4;

				SResultsData(double seed, unsigned int best_solution, unsigned int cv, double cpu_runtime):
					seed(seed),
					bestSolution(best_solution),
					constraintViolations(cv),
					cpuRunTime(cpu_runtime){

				}

				double ComputePRDP(unsigned int known_best){
					return 100*(((bestSolution+constraintViolations*CONSTRAINT_VIOLATION_PENALTY)-known_best)/known_best);
				}
	};

	static const std::string PIVOTING_RULE_BEST_IMPROVEMENT;
	static const std::string PIVOTING_RULE_FIRST_IMPROVEMENT;
	static const std::string NEIGHBORHOOD_TYPE_INSERT;
	static const std::string NEIGHBORHOOD_TYPE_TRANSPOSE;
	static const std::string NEIGHBORHOOD_TYPE_EXCHANGE;
	static const std::string NEIGHBORHOOD_CHAIN_TEI;
	static const std::string NEIGHBORHOOD_CHAIN_TIE;
	static const std::string VND_TYPE_STANDARD;
	static const std::string VND_TYPE_PIPED;
	static const std::string SEPARATOR;

	std::string m_sOutputFileName;
	unsigned int m_unKnownBest;
	std::list<SResultsData> m_listResults;
	std::ofstream m_ofsRResults;
	std::ofstream m_ofsTextResults;

};

#endif /* WRITER_H_ */
