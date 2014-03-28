#include "ball.h"

Ball::Ball()
{
}

void Ball::setBallPosition(Point ballPoint)
{
    ballPosition = ballPoint;
}

Point Ball::getBallPosition()
{
    return ballPosition;
}
