#include "model/ball.h"
#include "robot/robot.h"
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include <iostream>
using std::cout, std::endl;

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
    if(!robot) return false;
    Point bp = getPosition();
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
        robot->has_ball = false;

    if(!robot_with_ball || (!hasBall(robot_with_ball) && ++lastSeenWithoutBallCount > 10))
    {
        lastSeenWithoutBallCount = 0;
        robot_with_ball = nullptr;
        for(Robot* r : robots){
            if(hasBall(r)){
                robot_with_ball = r;
                robot_with_ball->has_ball = true;
                return;
            }
        }
    } else robot_with_ball->has_ball = true;


}

Robot* Ball::getRobotWithBall(){
    return robot_with_ball;
}
