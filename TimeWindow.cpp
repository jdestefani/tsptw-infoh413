/*
 * TimeWindow.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: deste
 */

#include "TimeWindow.h"

TimeWindow::TimeWindow() {
	lowerBound = 0;
	upperBound = 0;

}

TimeWindow::TimeWindow(unsigned int lower_bound, unsigned int upper_bound) {
	assert(upper_bound > lower_bound);
	lowerBound = lower_bound;
	upperBound = upper_bound;
}

TimeWindow::~TimeWindow() {
	// TODO Auto-generated destructor stub
}

bool TimeWindow::InTimeWindowBoundsIncluded(unsigned int value) {
	return lowerBound <= value && value <= upperBound;
}

bool TimeWindow::InTimeWindowBoundsExcluded(unsigned int value) {
	return lowerBound < value && value < upperBound;
}

bool TimeWindow::InTimeWindowUpperBoundIncluded(unsigned int value) {
	return lowerBound < value && value <= upperBound;
}

bool TimeWindow::InTimeWindowLowerBoundIncluded(unsigned int value) {
	return lowerBound <= value && value < upperBound;
}
