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
	TimeWindow();
	TimeWindow(unsigned int,unsigned int);
	virtual ~TimeWindow();
	bool InTimeWindowBoundsIncluded(unsigned int);
	bool InTimeWindowBoundsExcluded(unsigned int);
	bool InTimeWindowUpperBoundIncluded(unsigned int);
	bool InTimeWindowLowerBoundIncluded(unsigned int);
	unsigned int GetLowerBound() const;
	unsigned int GetUpperBound() const;

private:
	unsigned int lowerBound;
	unsigned int upperBound;
};

#endif /* TIMEWINDOW_H_ */
