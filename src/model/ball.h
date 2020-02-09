#ifndef BALL_H
#define BALL_H

#include "src/utilities/point.h"
class Robot;

class Ball {

public:
    /*! @brief Return a 2D point of the current ball location
     * \return A Point with the ball's position */
    static Point  getPosition();

    /*! @brief Return a 2D point of the X and Y component of the ball's velocity
     * \see Point
     * \see VelocityCalculator
     * \return The ball's velocity as an X/Y point in m/s*/
    static Point  getVelocity();

    /*! @brief Return a predicted stopping point for the ball
     * \return A Point with the ball's position */
    static Point  getStopPosition();

    /*! @brief Returns the ball's <i>speed</i> (magnitude of velocity) in m/s */
    static float  getSpeed();

    static void setPosition(Point);
    static void setVelocity(Point);
    static void setStopPosition(Point);


    static bool hasBall(Robot*);
    /*! @brief Returns the robot that currently has the ball
     * A robot has the ball if it is close to it and is facing it */
    static Robot* getRobotWithBall();
    //! @brief Sets the robot that has currently been determined to have the ball.
    static void setRobotWithBall();

private:

    static Point  position;           //The current point fo the ball on the field
    static Point  velocity;           //The current velocity of the ball on the field
    static Point  stopPosition;       //The predicted stop point of the ball
    static Point  predictedPosition;  //Prediciton point of the ball

    static Robot* robot_with_ball;


};


#endif // BALL_H
