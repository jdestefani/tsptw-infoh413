/*
 * TimeWindow.h
 *
 *  Created on: Mar 13, 2013
 *      Author: deste
 */

#ifndef TIMEWINDOW_H_
#define TIMEWINDOW_H_

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

	friend inline std::ostream& operator<<(std::ostream& output, const TimeWindow& tw){
			output << tw.m_unCityNumber << ") [" << tw.m_unLowerBound << "," << tw.m_unUpperBound << "]" << std::endl;
			return output;

	}

	inline bool operator<(TimeWindow &other) const{
		return m_unLowerBound < other.m_unLowerBound;
	}


private:
	unsigned int m_unLowerBound;
	unsigned int m_unUpperBound;
	unsigned int m_unCityNumber;
};

#endif /* TIMEWINDOW_H_ */
