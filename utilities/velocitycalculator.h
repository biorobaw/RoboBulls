#ifndef VEL_CALCULATOR_H
#define VEL_CALCULATOR_H

#include <deque>
#include "utilities/point.h"

/* Generic reusable struct to calculate a point's moving velocity over time.
 * The VelocityCalculator is an accumulator that maintains
 * the velocity of whatever point is passed to it. Returned is the new
 * velocity of the point in m/s. Create an instance of this class and
 * continually pass a point to it tomeasure its velocity.
 *
 * The `deque_size` parameter controls how many velocity readings to measure over
 * By default the current reading and the 10th oldest reading is used.
 */
 
/* The conversion factor for game "Points" to meters.
 * In the lab field there are about 1250 points / meter.
 * NOTE: This may not be true on the competition-size field.
 */
#define POINTS_PER_METER 1250
#define METERS_PER_POINT (1.0 / POINTS_PER_METER)

struct VelocityCalculator
{
    VelocityCalculator(unsigned deque_size = 10);
    Point update(const Point& movedPoint);

private:
    //Utility function to manage deque
    void addNewVelocityPoint(const Point&);

    /* Pair of Point and clock_t (time) values of size `maxSize` used to
     * store calculate velocity readings
     */
    unsigned maxSize;
    std::deque<std::pair<Point, clock_t>> velCalculations;
};


#endif
