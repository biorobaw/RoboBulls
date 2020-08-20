#include "model/ball.h"
#include "model/robot/robot.h"
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include <iostream>
using std::cout, std::endl;

Ball::~Ball(){

}

Point  Ball::getStopPosition(){
    return stopPosition;
}



void Ball::setStopPosition(Point p){
    stopPosition = p;
}





