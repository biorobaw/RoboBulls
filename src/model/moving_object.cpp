#define _USE_MATH_DEFINES
#include "moving_object.h"
#include <math.h>

namespace  {
    float flip_orientation(float o){
        o = M_PI - o;
        if(o >= M_PI) return o - M_2_PI;
        else if(o < -M_PI) return o + M_2_PI;
        return o;
    }
}

MovingObject::MovingObject()
{

}

MovingObject::~MovingObject(){

}


void MovingObject::copyFromSSLVision(MovingObject* object){
    in_field      = object->in_field;
    time_stamp    = object->time_stamp;
    if(!in_field) return;
    x             = object->x*flip_x;
    y             = object->y;
    velocity.x    = object->velocity.x*flip_x;
    velocity.y    = object->velocity.y;
    angular_speed = object->angular_speed*flip_x;

    if(flip_x==1) orientation = object->orientation;
    else orientation = flip_orientation(object->orientation);

}

bool MovingObject::isInField(){
    return in_field;
}

Point MovingObject::getPosition(){
    return Point(x,y);
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

void MovingObject::setFlipXCoordinates(bool do_flip_x){
    int new_value = do_flip_x ? -1 : 1;
    if(flip_x != new_value){
        flip_x = new_value;
        x *= -1;
        velocity.x *= -1;
        angular_speed *= -1;
        orientation = flip_orientation(orientation);


    }
}

int MovingObject::getFlipXCoordinates(){
    return flip_x;
}
