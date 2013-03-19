/*
 * CandidateSolution.h
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#ifndef CANDIDATESOLUTION_H_
#define CANDIDATESOLUTION_H_

#include <list>

class CandidateSolution {
public:
	CandidateSolution();
	virtual ~CandidateSolution();

	const std::list<unsigned int>& GetCityList() const;
	void SetCityList(const std::list<unsigned int>& listCitylist);
	unsigned int GetUnConstraintViolations() const;
	void SetUnConstraintViolations(unsigned int unConstraintViolations);
	unsigned int GetUnTourLength() const;
	void SetUnTourLength(unsigned int unTourLength);
	friend std::ostream& operator<<(std::ostream& out, const CandidateSolution& solution);

	void ComputeSolutionEvaluation();

private:
	std::list<unsigned int> m_listCitylist;
	unsigned int m_unTourLength;
	unsigned int m_unConstraintViolations;
	double m_fSolutionEvaluation;
};

#endif /* CANDIDATESOLUTION_H_ */
