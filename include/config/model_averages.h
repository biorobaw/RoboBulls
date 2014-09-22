#ifndef GAMEMODEL_AVERAGES_H
#define GAMEMODEL_AVERAGES_H

#include "include/config/simulated.h"
#include "include/config/globals.h"

/* This file contains options for the configuration
 * of the GameModel's (model/gamemodel) position averaging system.
 */

/* Defines whether or not the average system is used. Since this is an
 * experimental module (and transparent enough) it can  be disabled here.
 * Currently the average system has problems with angle-based tolerances.
 */
#define MODEL_USE_AVERAGES 0

/* Defines the max amount of averages to keep track of before
 * old averages are overwritten.
 * Defines the number of outliers that must be observed in a row
 * before a far-away point (>= OUTLIER_TOLERANCE) is re-considered
 * to be the point of the object. This is for cases in which
 * the object is intended to be moved a far distance, such as moving
 * the ball to a new point in the simulator.
 */
#if SIMULATED
 #define MAX_AVERAGES 5
 #define MAX_OUTLIERS 3
#else
 #define MAX_AVERAGES 16
 #define MAX_OUTLIERS 10
#endif

/* Defines the distance that a point must be away from the average
 * to be considered an outlier; any points outside this tolerance
 * will not be considered in the average.
 */
#define OUTLIER_POINT_TOLERANCE (5)
#define OUTLIER_ANGLE_TOLERANCE (0.5 * M_PI/180)

#endif
