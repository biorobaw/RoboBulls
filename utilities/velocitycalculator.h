#ifndef VEL_CALCULATOR_H
#define VEL_CALCULATOR_H

#include "utilities/point.h"

/* Generic reusable struct to calculate a point's moving velocity over time.
 * The VelocityCalculator is an accumulator that maintains
 * the velocity of whatever point is passed to it. Returned is the new
 * velocity of the point.
 * Create an instance of this class and continually pass a point to it to
 * measure its velocity
 */

struct VelocityCalculator
{
    Point update(const Point& movedPoint);
private:
    Point velocity, oldPoint;
};


#endif
