#include <iostream>
#include <sstream>
#include "point.h"

bool Point::operator==(const Point& rhs)
{
    return ((x == rhs.x) && (y == rhs.y));
}

bool Point::operator!=(const Point& rhs)
{
    return !(*this == rhs);
}

Point& Point::operator=(const Point& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Point& Point::operator+=(const Point& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point& Point::operator-=(const Point& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
    return *this;
}

Point& Point::operator*=(float rhs)
{
	this->x *= rhs;
	this->y *= rhs;
    return *this;
}

Point& Point::operator/=(float rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

Point Point::operator+(const Point& rhs)
{
    Point ret(x + rhs.x, y + rhs.y);
    return ret;
}

Point Point::operator-(const Point& rhs)
{
    Point ret(x - rhs.x, y - rhs.y);
    return ret;
}

Point Point::operator*(float rhs)
{
    Point ret(x * rhs, y * rhs);
    return ret;
}

/*! @brief Returns a string representation of the Point
 *  Example: "(x, y) = (543.0, -1123.8)" */
std::string Point::toString() const
{
    stringstream ss;
    ss << "(x, y)= " << "("<<x<<", "<<y<<")";
    return ss.str();
}
