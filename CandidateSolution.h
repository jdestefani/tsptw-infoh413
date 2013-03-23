/*
 * CandidateSolution.h
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#ifndef CANDIDATESOLUTION_H_
#define CANDIDATESOLUTION_H_

#include <vector>

class CandidateSolution {
public:
	CandidateSolution();
	CandidateSolution(std::vector<unsigned int>&);
	virtual ~CandidateSolution();

	const std::vector<unsigned int>& GetTour() const;
	void SetTour(const std::vector<unsigned int>& vecTour);
	unsigned int GetConstraintViolations() const;
	void SetConstraintViolations(unsigned int unConstraintViolations);
	unsigned int GetTourLength() const;
	void SetTourLength(unsigned int unTourLength);
	//friend std::ostream& operator<<(std::ostream& out, const CandidateSolution& solution);

	bool IsTourEqual(const std::vector<unsigned int>& listCitylist);
	void SwapSolutionComponents(unsigned int,unsigned int);
	void InsertSolutionComponent(unsigned int,unsigned int);
	void ComputeSolutionEvaluation();


private:
	std::vector<unsigned int> m_vecTour;
	unsigned int m_unTourLength;
	unsigned int m_unConstraintViolations;
	double m_fSolutionEvaluation;
};

#endif /* CANDIDATESOLUTION_H_ */
