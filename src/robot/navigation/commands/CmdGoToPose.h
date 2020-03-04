#ifndef GOTOPOSE_H
#define GOTOPOSE_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "utilities/point.h"

#define UNUSED_ANGLE_VALUE -10

struct CmdGoToPose {


public:
    static float default_velocity_multiplier;
    static float default_distance_tolerance;
    static float default_angle_tolerance;


public:
    //! @brief create a 'GoToPose' command given a target position and angle, optionally specify
    //! wheter to avoid obstacles and/or the ball
    CmdGoToPose(Point targetPose, float targetAngle,bool avoidObstacles = true, bool avoidBall = false);

    //! @brief create a 'GoToPose' command given a target position. Use this function when a
    //! target angle is not required. Optionally specify wheter to avoid obstacles and/or the ball
    CmdGoToPose(Point targetPose,bool avoidObstacles = true, bool avoidBall = false);

    CmdGoToPose(const CmdGoToPose& cmd);

    void setTarget(Point targetPose,float targetAngle = UNUSED_ANGLE_VALUE);
    bool hasTargetAngle();
    bool completed(Point currentPosition, float currentAngle);

    Point targetPose;
    float targetAngle;
    bool avoidObstacles;
    bool avoidBall;

    float velocity_multiplier = default_velocity_multiplier;

    float distance_tolerance = default_distance_tolerance;
    float angle_tolerance = default_angle_tolerance;


};

#endif // GOTOPOSE_H
