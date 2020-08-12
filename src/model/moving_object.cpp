#include "moving_object.h"

MovingObject::MovingObject()
{

}


bool MovingObject::isInField(){
    return in_field;
}

Point MovingObject::getPosition(){
    return position;
}
Point MovingObject::getVelocity(){
    return velocity;
}
float MovingObject::getSpeed(){
    return velocity.norm();
}

float MovingObject::getOrientation(){
    return orientation;
}
float MovingObject::getAngularSpeed(){
    return angular_speed;
}

float MovingObject::getTimeStamp(){
    return time_stamp;
}
