#include "behavior/rotateonpoint.h"

RotateOnPoint::RotateOnPoint(Point center, float faceAngle, float dist)
    : state(init)
    , center_point(center)
    , center_ang(faceAngle)
    , center_dist(dist)
    , finished(false)
    { }

void RotateOnPoint::perform(Robot *robot)
{
    float to_center = Measurments::angleBetween(robot, center_point);
    float to_robot  = Measurments::angleBetween(center_point, robot);

    switch(state)
    {
    case init: {
            float a = center_ang + M_PI;
            offset_point  = center_point + Point(cos(to_robot), sin(to_robot)) * center_dist;
              final_point = center_point + Point(cos(a), sin(a)) * center_dist;
               offset_ang = Measurments::angleBetween(center_point, offset_point);
            state = closing;
        }
        break;
    case closing: {
            setMovementTargets(offset_point, to_center);
            if(GenericMovementBehavior::isFinished()) {
                state = rotating;
                sign = difang_sign(offset_ang, center_ang + M_PI);
            }
        }
        break;
    case rotating: {
            if(not finished) {
                float next = Measurments::angleSum(to_robot, sign * 45*(M_PI/180));
                Point offset = center_point + Point(cos(next), sin(next)) * center_dist;
                setMovementTargets(offset, to_center);
                if(Measurments::isClose(robot->getRobotPosition(), final_point)) {
                    finished = true;
                    setMovementTargets(robot->getRobotPosition(), to_center);
                }
            }
        }
        break;
    }

    GenericMovementBehavior::perform(robot, Movement::Type::facePoint);
}

bool RotateOnPoint::isFinished()
{
    return finished;
}

//See http://stackoverflow.com/questions/1878907
double RotateOnPoint::difang_sign(double x, double y)
{
    double arg = fmod(y-x, 2*M_PI);
    if (arg < 0 )  arg  = arg + 2*M_PI;
    if (arg > M_PI) arg = arg - 2*M_PI;
    return (arg) / abs(arg);
}

