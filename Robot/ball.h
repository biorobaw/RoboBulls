#ifndef BALL_H
#define BALL_H

#include "Measure/point.h"

class Ball
{
public:
    Ball();

    //sets
    void setBallPosition(Point);

    //gets
    Point getBallPosition();

private:
    Point ballPosition;
};

#endif // BALL_H
