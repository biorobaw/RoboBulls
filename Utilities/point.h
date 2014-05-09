#ifndef POINT_H
#define POINT_H

#include <string>
#include <iostream>

#include <sstream>

using namespace std;

class Point
{
public:
    Point() : x(0), y(0) {}
    Point(float nx, float ny)/* : x(nx), y(ny)*/ {x = nx; y = ny;}

    bool	operator==(const Point& rhs);
    bool	operator!=(const Point& rhs);
    Point&	operator=(const Point& rhs);
    Point	operator+(const Point& rhs);
    Point	operator-(const Point& rhs);
    stringstream& toString();
public:
    float x, y;
};


#endif // POINT_H
