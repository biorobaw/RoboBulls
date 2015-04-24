#ifndef VEL_CALCULATOR_H
#define VEL_CALCULATOR_H

#include "utilities/point.h"

/* Generic reusable struct to calculate a point's moving velocity over time.
 * The VelocityCalculator is an accumulator that maintains
 * the velocity of whatever point is passed to it. Returned is the new
 * velocity of the point. Value retuned is in m/s
 * Create an instance of this class and continually pass a point to it to
 * measure its velocity
 */
 
/* The conversion factor for game "Points" to meters.
 * In the lab field there are about 1250 points / meter.
 * NOTE: This may not be true on the competition-size field.
 */
#define POINTS_PER_METER 1250
#define METERS_PER_POINT (1.0 / POINTS_PER_METER)

/* Defines the default minimum distance, in Points, a point must move
 * To be considered having velocity.
 */
#define NOISE_DIST_THRESHOLD 16


struct VelocityCalculator
{
    VelocityCalculator(float dist_threshold = NOISE_DIST_THRESHOLD);
    
    Point update(const Point& movedPoint);
private:
    float threshold;
    Point oldPoint;
    Point oldVelocity;
};


#endif
