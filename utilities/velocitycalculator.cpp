#include <time.h>
#include "utilities/measurments.h"
#include "utilities/velocitycalculator.h"

/* Defines the minimum distance a point must move
 * To be considered having velocity.
 */
#define NOISE_DIST_THRESHOLD 10

/* The conversion factor for game "Points" to meters.
 * In the lab field there are about 1250 points / meter.
 * NOTE: This may not be true on the competition-size field.
 */
#define POINTS_PER_METER 1250


Point VelocityCalculator::update(const Point& movedPoint)
{
    static time_t now = clock(), prev = 0;

    if(Measurments::distance(oldPoint, movedPoint) > NOISE_DIST_THRESHOLD)
    {
        prev = now;
        now = clock();
        float changeInSec = (float)(now - prev) / CLOCKS_PER_SEC;
        float velocityX = ((movedPoint.x - oldPoint.x) / POINTS_PER_METER) / changeInSec;
        float velocityY = ((movedPoint.y - oldPoint.y) / POINTS_PER_METER) / changeInSec;
        oldPoint = movedPoint;
        velocity = Point(velocityX, velocityY);
    }

    return velocity;
}
