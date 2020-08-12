#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H
#include "utilities/point.h"

class MovingObject
{
public:
    MovingObject();

    bool isInField();

    Point getPosition();
    Point getVelocity();
    float getSpeed();

    float getOrientation();
    float getAngularSpeed();

    float getTimeStamp();

protected:
    friend class SSLVisionListener;

    bool in_field = false;      // defines whether the object is currently on the field or not


    Point position = Point(0,0); // position in millimeters
    Point velocity = Point(0,0); // velocity in millimeters per second

    float orientation = 0;       // orientation of the object in radians
    float angular_speed = 0;     // angular speed in radiands per second


    float time_stamp=0;     // time stamp of last detection

};

#endif // MOVINGOBJECT_H
