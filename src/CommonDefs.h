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

      Version: 1.1
      File:    CommonDefs.h
      Author:  Jacopo De Stefani
      Purpose: Header file containing global data types definitions
      	  	   used by the different modules
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


#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_
#include <time.h>

typedef enum {
	RANDOM,
	HEURISTIC
} EInitFunction;

typedef enum {
	NO_VND,
	STANDARD_VND,
	PIPED_VND,
}EVNDType;

typedef enum {
	TRANSPOSE,
	EXCHANGE,
	INSERT
} ENeighborhoodType;

typedef enum {
	NO_CHAIN,
	TRANSPOSE_EXCHANGE_INSERT,
	TRANSPOSE_INSERT_EXCHANGE,
} ENeighborhoodChain;

typedef enum {
	BEST_IMPROVEMENT,
	FIRST_IMPROVEMENT
} ESolutionUpdate;

typedef enum {
	ANT_COLONY_OPTIMIZATION,
	SIMULATED_ANNEALING
} ESLSType;


/*
      METHOD:         Computation of the time passed by two time instants.
      INPUT:          struct timespec containing the starting and ending time of the period to compute
      OUTPUT:         none
      (SIDE)EFFECTS:  Modifies the state of the class
*/
inline double ComputeRunTime(struct timespec& s_begin_time, struct timespec& s_end_time) {
	struct timespec temp;
		if ((s_end_time.tv_nsec-s_begin_time.tv_nsec)<0) {
			temp.tv_sec = s_end_time.tv_sec-s_begin_time.tv_sec-1;
			temp.tv_nsec = 1000000000+s_end_time.tv_nsec-s_begin_time.tv_nsec;
		} else {
			temp.tv_sec = s_end_time.tv_sec-s_begin_time.tv_sec;
			temp.tv_nsec = s_end_time.tv_nsec-s_begin_time.tv_nsec;
		}
	return temp.tv_sec+temp.tv_nsec/(10e9);
}

const unsigned int SAMPLESPERINTERVAL = 50;
const int LOGAXISLOWERBOUND = -2;
const int LOGAXISUPPERBOUND = 2;
const int PENALTY = 10e4;
#endif /* COMMONDEFS_H_ */
