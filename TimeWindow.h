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
				   m_unUpperBound(0){
	}

	TimeWindow(unsigned int lower_bound, unsigned int upper_bound) : m_unLowerBound(lower_bound),
																	 m_unUpperBound(upper_bound){
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

private:
	unsigned int m_unLowerBound;
	unsigned int m_unUpperBound;
};

#endif /* TIMEWINDOW_H_ */
