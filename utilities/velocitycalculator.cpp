#include <time.h>
#include "include/config/globals.h"
#include "utilities/velocitycalculator.h"

VelocityCalculator::VelocityCalculator(unsigned deque_size)
    : maxSize(deque_size)
    { }

Point VelocityCalculator::update(const Point& movedPoint)
{
    addNewVelocityPoint(movedPoint);

    // Difference of most recent reading and oldest
    Point displacement = velCalculations.back().first - velCalculations.front().first;
    clock_t time = velCalculations.back().second - velCalculations.front().second;

    // Calculate change displacement over seconds taken from above
    float changeInSec = (float)(time) / CLOCKS_PER_SEC;
    float velocityX = (displacement.x / POINTS_PER_METER) / changeInSec;
    float velocityY = (displacement.y / POINTS_PER_METER) / changeInSec;

    return Point(velocityX, velocityY);
}

void VelocityCalculator::addNewVelocityPoint(const Point& movedPoint)
{
    //Push back new point and remove oldest if size hit
    if(velCalculations.size() == maxSize) {
        velCalculations.pop_front();
    }
    velCalculations.emplace_back(std::make_pair(movedPoint, clock()));
}
