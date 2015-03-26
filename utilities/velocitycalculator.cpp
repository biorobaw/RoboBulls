#include <time.h>
#include "utilities/measurments.h"
#include "utilities/velocitycalculator.h"
#include "utilities/debug.h"

//The number of velocities that the calculator will hold
#define VEL_COLLECTIONS_MAX 10

//Calculates new average velocity every SAMPLE_TIME_MAX+1 runs.
#define SAMPLE_TIME_MAX     1


VelocityCalculator::VelocityCalculator(float dist_threshold)
    : threshold(dist_threshold)
    , sampleTimer(0)
    { }

Point VelocityCalculator::update(const Point& movedPoint)
{
    static time_t now = clock(), prev = 0;

    if(++sampleTimer > SAMPLE_TIME_MAX)
    {
        //Average over all collections in the average deque and and return resuls
        sampleTimer = 0;
        avgVelocity = Point(0, 0);
        for(const Point& pt : velCollection) {
            avgVelocity += pt;
        }
        avgVelocity /= VEL_COLLECTIONS_MAX;
    }
    else {
        //Measures changes in time since last sample and calculates velocity.
        prev = now;
        now = clock();
        float changeInSec = (float)(now - prev) / CLOCKS_PER_SEC;
        float velocityX = ((movedPoint.x - oldPoint.x) / POINTS_PER_METER) / changeInSec;
        float velocityY = ((movedPoint.y - oldPoint.y) / POINTS_PER_METER) / changeInSec;
        oldPoint = movedPoint;
        Point newVelocity = Point(velocityX, velocityY);

        //Pop average deque if reached size, also put the new one in the back
        if(velCollection.size() == VEL_COLLECTIONS_MAX) {
            velCollection.pop_front();
        }
        if(velCollection.size() < VEL_COLLECTIONS_MAX) {
            velCollection.push_back(newVelocity);
        }
    }

    return avgVelocity;
}
