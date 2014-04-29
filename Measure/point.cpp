#include "point.h"

bool Point::operator==(const Point& rhs)
{
    return ((this->x == rhs.x) &&
            (this->y == rhs.y));
}


bool Point::operator!=(const Point& rhs)
{
    return !(*this == rhs);
}

Point& Point::operator=(const Point& rhs)
{
    this->x = rhs.x;
    this->y = rhs.y;
    return *this;
}

Point Point::operator+(const Point& rhs)
{
    Point ret(this->x + rhs.x, this->y + rhs.y);
    return ret;
}

Point Point::operator-(const Point& rhs)
{
    Point ret(this->x - rhs.x, this->y - rhs.y);
    return ret;
}

string Point::toString()
{
//    stringstream myString;
//    cout<<"trying to access x and y"<<endl;
    cout<<"(x, y)= "<<"("<<x << ", "<<y<<" )";
//    myString << "(x, y)= " << "("<<x<<", "<<y<<")";
//    return myString.str();
    return "";
}

/*
 * Old: float x = Point.getX();
 * New: float x = Point.x;
 */
