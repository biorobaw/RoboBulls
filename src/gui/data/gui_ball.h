#ifndef GUI_BALL_H
#define GUI_BALL_H

#include "utilities/point.h"

//Proxy between the gui and the ball

class GuiBall
{
public:
    static GuiBall ball;
    static void updateBall();

    static Point getPosition();
    static Point getVelocity();
    static float getSpeed();


protected:

    Point position = Point(0,0);
    Point velocity = Point(0,0);
    float speed    = 0;

private:
    GuiBall();

};

#endif // GUI_BALL_H
