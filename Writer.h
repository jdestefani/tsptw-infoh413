/*
 * Writer.h
 *
 *  Created on: Mar 22, 2013
 *      Author: deste
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <list>
#include <string>
#include <fstream>

class Writer {
public:

	Writer(std::string,unsigned int);
	virtual ~Writer();

	void OpenRFile();
	void OpenTextResults();
	void FlushRFile();
	void FlushTextResults();
	void AddData(double,unsigned int,unsigned int,double);


private:

	struct SResultsData{
				double seed;
				unsigned int bestSolution;
				unsigned int constraintViolations;
				double cpuRunTime;

				SResultsData(double seed, unsigned int best_solution, unsigned int cv, double cpu_runtime):
					seed(seed),
					bestSolution(best_solution),
					constraintViolations(cv),
					cpuRunTime(cpu_runtime){

				}

				double ComputePRDP(unsigned int known_best){
					return 100*(((bestSolution+constraintViolations)-known_best)/known_best);
				}
	};

	std::string m_sInstanceName;
	unsigned int m_unKnownBest;
	std::list<SResultsData> m_listResults;
	std::ofstream m_ofsRResults;
	std::ofstream m_ofsTextResults;
};

#endif /* WRITER_H_ */
