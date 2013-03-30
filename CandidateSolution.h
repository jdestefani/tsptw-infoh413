/*
 * CandidateSolution.h
 *
 *  Created on: Mar 18, 2013
 *      Author: deste
 */

#ifndef CANDIDATESOLUTION_H_
#define CANDIDATESOLUTION_H_

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

class CandidateSolution {
public:
	CandidateSolution() :
		m_unTourDuration(0),
		m_unConstraintViolations(0),
		m_fSolutionEvaluation(0.0f){
		m_vecTour.reserve(1);
	}

	CandidateSolution(std::vector<unsigned int>& tour):
		m_vecTour(tour),
		m_unTourDuration(0),
		m_unConstraintViolations(0),
		m_fSolutionEvaluation(0.0f){
	}
	virtual ~CandidateSolution();

	inline double GetSolutionEvaluation() const {
		return m_fSolutionEvaluation;
	}

	inline void SetSolutionEvaluation(double fSolutionEvaluation) {
		m_fSolutionEvaluation = fSolutionEvaluation;
	}

	inline unsigned int GetConstraintViolations() const {
		return m_unConstraintViolations;
	}

	inline void SetConstraintViolations(unsigned int unConstraintViolations) {
		m_unConstraintViolations = unConstraintViolations;
	}

	inline unsigned int GetTourDuration() const {
		return m_unTourDuration;
	}

	inline void SetTourDuration(unsigned int unTourDuration) {
		m_unTourDuration = unTourDuration;
	}

	const std::vector<unsigned int>& GetTour() const {
		return m_vecTour;
	}

	void SetTour(const std::vector<unsigned int>& vecTour) {
		m_vecTour = vecTour;
		}

	friend inline std::ostream& operator<<(std::ostream& output, const CandidateSolution& solution){
		output << "Tour: ";
		for(unsigned int i = 0; i < solution.GetTour().size(); i++){
			output << solution.GetTour().at(i) << " ";
		}
		output << std::endl;
		output << "Constraint Violations: " << solution.GetConstraintViolations() << std::endl;
		output << "Total travel time: " << solution.GetTourDuration() << std::endl;
		return output;

	}

	bool IsTourEqual(const std::vector<unsigned int>& listCitylist);
	void SwapSolutionComponents(unsigned int,unsigned int);
	void InsertSolutionComponent(unsigned int,unsigned int);
	void ComputeSolutionEvaluation();
	std::string ToString();



private:
	std::vector<unsigned int> m_vecTour;
	unsigned int m_unTourDuration;
	unsigned int m_unConstraintViolations;
	double m_fSolutionEvaluation;
};

#endif /* CANDIDATESOLUTION_H_ */
