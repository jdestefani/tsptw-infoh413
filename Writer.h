/*
 * Writer.h
 *
 *  Created on: Mar 22, 2013
 *      Author: deste
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <list>

class Writer {
public:
	Writer();
	virtual ~Writer();

	void OpenRFile();
	void OpenTextResults();
	void FlushRFile();
	void FlushTextResults();
	void AddData(double,unsigned int,double);
	double ComputePRDP(double,unsigned int);

private:
	std::string m_sInstanceName;
	unsigned int m_unKnownBest;
	std::list<unsigned int> m_listBestSolutions;
	std::list<unsigned int> m_listConstraintViolations;
	std::list<double> m_listCpuRunTimes;
	std::list<double> m_listPenalizedRelativePercentageDeviations;
	std::ofstream m_ofsRResults;
	std::ofstream m_ofsTextResults;
};

#endif /* WRITER_H_ */
