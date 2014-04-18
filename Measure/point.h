#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point() : x(0), y(0) {}
    Point(float nx, float ny) : x(nx), y(ny) {}

    bool	operator==(const Point& rhs);
    bool	operator!=(const Point& rhs);
    Point&	operator=(const Point& rhs);
    Point	operator+(const Point& rhs);
    Point	operator-(const Point& rhs);

public:
    float x, y;
};


#endif // POINT_H
