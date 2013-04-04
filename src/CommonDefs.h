/*
 * CommonDefs.h
 *
 *  Created on: Apr 2, 2013
 *      Author: development
 */

#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_


typedef enum {
			RANDOM,
			HEURISTIC
		} EInitFunction;

typedef enum {
		EXCHANGE,
		TRANSPOSE,
		INSERT
		} ENeighborhoodType;

typedef enum {
		BEST_IMPROVEMENT,
		FIRST_IMPROVEMENT
		}ESolutionUpdate;

#endif /* COMMONDEFS_H_ */
