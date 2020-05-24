#ifndef GUI_BALL_H
#define GUI_BALL_H

#include <QObject>
#include "utilities/point.h"

//Proxy between the gui and the ball

class GuiBall : public QObject
{
    Q_OBJECT
public:
    static GuiBall ball;      // singleton instance of the ball
    static void updateBall(); // function to update the ball information

    static Point getPosition(); // returns the current position of the ball
    static Point getVelocity(); // returns the current velocity of the ball
    static float getSpeed();    // returns the current speed of the ball

signals:
    void color_changed();

protected:

    Point position = Point(0,0); // current position of the ball
    Point velocity = Point(0,0); // current velocity of the ball
    float speed    = 0;          // current speed of the ball

private:
    GuiBall();

};

#endif // GUI_BALL_H
