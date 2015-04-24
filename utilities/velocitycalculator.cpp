#include <time.h>
#include "utilities/measurments.h"
#include "utilities/velocitycalculator.h"
#include "utilities/debug.h"

//The number of velocities that the calculator will hold
#define VEL_COLLECTIONS_MAX 10

//Calculates new average velocity every SAMPLE_TIME_MAX+1 runs.
#define SAMPLE_TIME_MAX     0


VelocityCalculator::VelocityCalculator(float dist_threshold)
    : threshold(dist_threshold)
    { }

Point VelocityCalculator::update(const Point& movedPoint)
{
    static time_t now = clock(), prev = 0;

    //Measures changes in time since last call and calculates velocity.
    prev = now;
    now = clock();
    float changeInSec = (float)(now - prev) / CLOCKS_PER_SEC;
    float velocityX = ((movedPoint.x - oldPoint.x) / POINTS_PER_METER) / changeInSec;
    float velocityY = ((movedPoint.y - oldPoint.y) / POINTS_PER_METER) / changeInSec;
    oldPoint = movedPoint;

    float magOld = std::hypot(oldVelocity.x, oldVelocity.y);
    Point newVelocity = Point(velocityX, velocityY);
    float magNew = std::hypot(newVelocity.x, newVelocity.y);

    //Disregard changes in velocity that look too big
    if(abs(magNew - magOld) > 2 || magNew > 6) {
        newVelocity = oldVelocity;
    }

    oldVelocity = newVelocity;
    return newVelocity;
}
