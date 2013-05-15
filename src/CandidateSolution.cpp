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
      File:    CandidateSolution.cpp
      Author:  Jacopo De Stefani
      Purpose: Implementation file for the user defined class to
      	  	   model a candidate solution of the problem.
      	  	   Contains all the basic operations on a candidate solution:
      	  	   -> Exchange components
      	  	   -> Insert components
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

#include "CandidateSolution.h"



CandidateSolution::~CandidateSolution() {
	// TODO Auto-generated destructor stub
}

bool CandidateSolution::IsTourEqual(const std::vector<unsigned int>& external_tour) {
	unsigned int i=0;
	unsigned int j=0;

	while(i < m_vecTour.size() && j < external_tour.size())
	{
		if(m_vecTour.at(i) != external_tour.at(j)){
			return false;
		}
		i++;
		j++;
	}
	return true;
}

void CandidateSolution::SwapSolutionComponents(unsigned int firstIndex,unsigned int secondIndex) {
	if(firstIndex != secondIndex){
		unsigned int swapVariable = m_vecTour.at(secondIndex);
		m_vecTour.at(secondIndex) = m_vecTour.at(firstIndex);
		m_vecTour.at(firstIndex) = swapVariable;
	}

}

void CandidateSolution::InsertSolutionComponent(unsigned int city_index, unsigned int insertion_position) {
	if(city_index != insertion_position){
		unsigned int elementToInsert = m_vecTour.at(city_index);
		if(insertion_position > city_index){
			for(unsigned int i=city_index;i<insertion_position;i++){
				m_vecTour.at(i) = m_vecTour.at(i+1);
			}
		}
		else{
			for(unsigned int i=city_index;i>insertion_position;i--){
				m_vecTour.at(i) = m_vecTour.at(i-1);
			}
		}
		m_vecTour.at(insertion_position) = elementToInsert;
	}
}

void CandidateSolution::Reset() {
	m_vecTour.erase(m_vecTour.begin(),m_vecTour.end());
	m_unTourDuration = 0;
	m_unConstraintViolations = 0;
}

double CandidateSolution::ComputeRelativeSolutionQuality(double known_best) {
	return (double(m_unTourDuration + PENALTY*m_unConstraintViolations)/known_best)-1;
}

std::string CandidateSolution::ToString() {
	std::stringstream outputString;
	outputString << "Tour: ";
	for(unsigned int i = 0; i < m_vecTour.size(); i++){
		outputString << m_vecTour.at(i) << " ";
	}
	outputString << std::endl;
	outputString << "Constraint Violations: " << m_unConstraintViolations << std::endl;
	outputString << "Total travel time: " << m_unTourDuration << std::endl;
	return outputString.str();
 }

void CandidateSolution::ComputeSolutionEvaluation() {
	m_lfSolutionEvaluation = m_unTourDuration + PENALTY*m_unConstraintViolations;
}


