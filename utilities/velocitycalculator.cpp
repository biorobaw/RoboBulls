#include <time.h>
#include "utilities/measurments.h"
#include "utilities/velocitycalculator.h"

VelocityCalculator::VelocityCalculator(float dist_threshold)
    : threshold(dist_threshold)
    { }

Point VelocityCalculator::update(const Point& movedPoint)
{
    static time_t now = clock(), prev = 0;

    if(Measurments::distance(oldPoint, movedPoint) > threshold)
    {
        prev = now;
        now = clock();
        float changeInSec = (float)(now - prev) / CLOCKS_PER_SEC;
        float velocityX = ((movedPoint.x - oldPoint.x) / POINTS_PER_METER) / changeInSec;
        float velocityY = ((movedPoint.y - oldPoint.y) / POINTS_PER_METER) / changeInSec;
        oldPoint = movedPoint;
        velocity = Point(velocityX, velocityY);

        //Reasonable velocity check
        if(velocity.x > 8.0 || velocity.y > 8.0)
            velocity = Point(0,0);
    }

    return velocity;
}
