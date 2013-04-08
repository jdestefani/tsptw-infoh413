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
      File:    TimeWindow.h
      Author:  Jacopo De Stefani
      Purpose: Header file for user defined class to model time windows
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

#ifndef TIMEWINDOW_H_
#define TIMEWINDOW_H_

#include <cstdlib>
#include <assert.h>

class TimeWindow {
public:

	TimeWindow() : m_unLowerBound(0),
				   m_unUpperBound(0),
				   m_unCityNumber(0){
	}

	TimeWindow(unsigned int lower_bound, unsigned int upper_bound, unsigned int city_number) :
		m_unLowerBound(lower_bound),
		m_unUpperBound(upper_bound),
		m_unCityNumber(city_number){
		assert(upper_bound > lower_bound);
	}

	~TimeWindow() {
		// TODO Auto-generated destructor stub
	}

	inline bool InTimeWindowBoundsIncluded(unsigned int value) {
		return m_unLowerBound <= value && value <= m_unUpperBound;
	}

	inline bool InTimeWindowBoundsExcluded(unsigned int value) {
		return m_unLowerBound < value && value < m_unUpperBound;
	}

	inline bool InTimeWindowUpperBoundIncluded(unsigned int value) {
		return m_unLowerBound < value && value <= m_unUpperBound;
	}

	inline bool InTimeWindowm_unLowerBoundIncluded(unsigned int value) {
		return m_unLowerBound <= value && value < m_unUpperBound;
	}

	inline unsigned int GetLowerBound() const {
		return m_unLowerBound;
	}

	inline unsigned int GetUpperBound() const {
		return m_unUpperBound;
	}

	inline unsigned int GetCityNumber() const {
			return m_unCityNumber;
	}

	friend inline std::ostream& operator<<(std::ostream& output, const TimeWindow& tw){
			output << tw.m_unCityNumber << ") [" << tw.m_unLowerBound << "," << tw.m_unUpperBound << "]" << std::endl;
			return output;

	}

	inline bool operator<(const TimeWindow &other) const{
		return m_unUpperBound < other.m_unUpperBound;
		//return float(rand())/RAND_MAX < 0.6 ?  m_unUpperBound < other.m_unUpperBound : m_unLowerBound < other.m_unUpperBound;
	}


private:
	unsigned int m_unLowerBound;
	unsigned int m_unUpperBound;
	unsigned int m_unCityNumber;
};

#endif /* TIMEWINDOW_H_ */
