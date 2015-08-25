#ifndef VEL_CALCULATOR_H
#define VEL_CALCULATOR_H
#include <deque>
#include "utilities/point.h"

//! @file

//! @brief Conversion factor from meters to field Point coordinates
#define POINTS_PER_METER 1250

//! @brief Conversion factor from Points to meters
#define METERS_PER_POINT (1.0 / POINTS_PER_METER)

/*! @brief Generic object to calculate velocity of a Point over time.
 * @author JamesW
 *
 * The VelocityCalculator is an accumulator queue that maintains
 * a rolling velocity of whatever point is passed to it. Returned is the
 * new velocity of the point in <b>m/s</b>, which is taken over the
 * front and the end of the queue. Create an instance of this class
 * and continually pass a point to it tomeasure its velocity.
 * Use POINTS_PER_METER to convert the returnes m/s into Points per second
 */

class VelocityCalculator
{
public:
    /*! @brief Constructor
     *  @details A smaller deque_size can provide more up-to-date readings at the
     *  cost of incresed spikes and noise.
     *  @param deque_size controls how many velocity readings to measure over.
     *  By default a 10 readings are stored */
    VelocityCalculator(unsigned deque_size = 10);

    /*! @brief Updates the accumulator queue with a new point
     *  @return The observed change in the point */
    Point update(const Point& movedPoint);

private:
    //Utility function to manage deque
    void addNewVelocityPoint(const Point&);

    /* Pair of Point and clock_t (time) values of size `maxSize` used to
     * store calculate velocity readings */
    unsigned maxSize;
    std::deque<std::pair<Point, clock_t>> velCalculations;
};


#endif
