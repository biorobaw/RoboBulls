#include <time.h>
#include "parameters/field.h"
#include "utilities/velocitycalculator.h"

VelocityCalculator::VelocityCalculator(unsigned deque_size)
    : max_size(deque_size)
    { }

void VelocityCalculator::update(const Point& movedPoint)
{
    //Push back new point and remove oldest if size hit
    if(vel_history.size() == max_size) {
        vel_history.pop_front();
    }
    vel_history.emplace_back(std::make_pair(movedPoint, clock()));
}

Point VelocityCalculator::getVelocityMetersPerSecond(){
    // Difference of most recent reading and oldest
    Point displacement = vel_history.back().first - vel_history.front().first;
    clock_t time = vel_history.back().second - vel_history.front().second;

    // Calculate change displacement over seconds taken from above
    float changeInSec = (float)(time) / CLOCKS_PER_SEC;
    float velocityX = (displacement.x / POINTS_PER_METER) / changeInSec;
    float velocityY = (displacement.y / POINTS_PER_METER) / changeInSec;

    return Point(velocityX, velocityY);
}

Point VelocityCalculator::getVelocityMillimetersPerFrame(){
    // Difference of most recent reading and oldest
    Point displacement = vel_history.back().first - vel_history.front().first;

    // Calculate change displacement over seconds taken from above
    float velocityX = (displacement.x) / max_size;
    float velocityY = (displacement.y) / max_size;

    return Point(velocityX, velocityY);
}
