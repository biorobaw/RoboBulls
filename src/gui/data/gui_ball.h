#ifndef GUI_BALL_H
#define GUI_BALL_H

#include <QObject>
#include "utilities/point.h"

//Proxy between the gui and the ball
class GameState;
class GuiBall : public QObject
{
    Q_OBJECT
public:
    static Point getPosition(); // returns the current position of the ball
    static Point getVelocity(); // returns the current velocity of the ball
    static float getSpeed();    // returns the current speed of the ball

    static GuiBall* gui_ball;

signals:
    void color_changed();

protected:

    static GameState* game_state;

private:
    GuiBall();

};

#endif // GUI_BALL_H
