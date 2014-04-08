#ifndef POINT_H
#define POINT_H

class Point
{
public:
    //constructor
    Point();

    //sets
    void setX(float);
    void setY(float);

    //gets
    float getX();
    float getY();



private:
    float x, y;
};

#endif // POINT_H
