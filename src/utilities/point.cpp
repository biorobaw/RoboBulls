#include <iostream>
#include <sstream>
#include "point.h"
#include <math.h>
#include <QDebug>


QDebug operator<<(QDebug debug, const Point& p){
    QDebugStateSaver saver(debug);
    debug.nospace() << "(" << p.x << "," << p.y << ")";
    return debug;
}

Point::Point(float angle) :
    x(cos(angle)), y(sin(angle))
{

}

Point::Point(const Point& p){
    x = p.x;
    y = p.y;
}

Point::~Point(){

}

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

Point Point::operator+(const Point& rhs) const
{
    Point ret(x + rhs.x, y + rhs.y);
    return ret;
}

Point Point::operator-(const Point& rhs) const
{
    Point ret(x - rhs.x, y - rhs.y);
    return ret;
}

Point Point::operator*(float rhs) const
{
    Point ret(x * rhs, y * rhs);
    return ret;
}

Point Point::operator/(float rhs) const
{
    Point ret(x / rhs, y / rhs);
    return ret;
}

float Point::norm2() const{
    return x*x+y*y;
}

Point Point::normalized() const{
    float length = norm();
    return Point(x/length, y/length);
}

Point Point::normalizedPerpen() const {
    float length = norm();
    return Point(-y/length, x/length);
}

float Point::norm() const{
    return sqrt(x*x+y*y);
}


float Point::dot(const Point& rhs) const{
    return x*rhs.x + y*rhs.y;
}
float Point::wedge(const Point& rhs) const{
    return x*rhs.y - y*rhs.x;
}

float Point::angle() const{
    return atan2(y,x);
}

Point Point::perpen() const{
    return Point(-y,x);
}



Point Point::complexMultiplication(const Point& rhs) const {
    return Point(x*rhs.x-y*rhs.y, x*rhs.y+y*rhs.x);
}

Point Point::rotate(float angle) const {
    return this->complexMultiplication(Point(angle));
}

/*! @brief Returns a string representation of the Point
 *  Example: "(x, y) = (543.0, -1123.8)" */
std::string Point::toString() const
{
    std::stringstream ss;
    ss << "(x, y)= " << "("<<x<<", "<<y<<")";
    return ss.str();
}
