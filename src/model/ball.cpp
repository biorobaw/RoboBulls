#include "src/model/ball.h"
#include "src/model/robot.h"
#include "src/utilities/measurements.h"
#include "src/utilities/comparisons.h"

Point Ball::position = Point(0,0);
Point Ball::velocity = Point(0,0);
Point Ball::stopPosition = Point(0,0);
Point Ball::predictedPosition = Point(0,0);
Robot* Ball::robot_with_ball = nullptr;


Point  Ball::getPosition(){
    return position;
}
Point  Ball::getVelocity(){
    return velocity;
}
Point  Ball::getStopPosition(){
    return stopPosition;
}
float  Ball::getSpeed(){
    return Measurements::mag(velocity);
}

void Ball::setPosition(Point p){
    position = p;
}
void Ball::setVelocity(Point v){
    velocity = v;
}
void Ball::setStopPosition(Point p){
    stopPosition = p;
}


bool Ball::hasBall(Robot* robot)
{
    Point bp = getPosition();
    if(!robot)
        return false;
    return Comparisons::isDistanceToLess(robot, bp, 300) &&
           Comparisons::isFacingPoint(robot, bp);
}

// TODO: the function returns first match, not best match
void Ball::setRobotWithBall(){
    //Count of how many times `robotWithBall` has been seen without ball
    static int lastSeenWithoutBallCount = 0;

    //Assume no robot has the ball first
    auto robots = Robot::getAllRobots();
    for(Robot* robot : robots)
        robot->hasBall = false;

    if(!hasBall(robot_with_ball) && ++lastSeenWithoutBallCount > 10)
    {
        lastSeenWithoutBallCount = 0;
        robot_with_ball = nullptr;
        for(Robot* r : robots){
            if(hasBall(r)){
                robot_with_ball = r;
                break;
            }
        }
    }

    if(robot_with_ball) robot_with_ball->hasBall = true;
}

Robot* Ball::getRobotWithBall(){
    return robot_with_ball;
}
