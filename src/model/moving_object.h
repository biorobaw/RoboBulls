#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H
#include "utilities/point.h"

class MovingObject : public Point // inherits x y coordinates + all operations with points
{
public:
    MovingObject();
    virtual ~MovingObject();
    void copyFromSSLVision(MovingObject* object); // use this only from ssl vision

    bool isInField();

    Point getPosition();
    Point getVelocity();
    float getSpeed();

    float getOrientation();
    float getAngularSpeed();

    float getTimeStamp();


    /*! setFlipXCoordinates is used to simplify team control logic assuming
        teams are in the negative side of the field
        if theyr are not, coordinates are flipped for the logic and then
        fixed again before sending the speeds to the robots */
    void setFlipXCoordinates(bool do_flip_x);
    virtual int getFlipXCoordinates();

    using Point::operator=;

protected:
    friend class SSLVisionListener;

    bool in_field = false;      // defines whether the object is currently on the field or not

    Point velocity = Point(0,0); // velocity in millimeters per second

    float orientation = 0;       // orientation of the object in radians
    float angular_speed = 0;     // angular speed in radiands per second

    float time_stamp=0;  // time stamp of last detection

private:
    int flip_x = 1; /* either 1 or -1, used to simplify team logic by assuming the team is
                       in the neagtive field side during the control logic, and then fixing
                       the speeds before sending them to the robots */
};

#endif // MOVINGOBJECT_H
