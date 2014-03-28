#include "measurments.h"

Measurments::Measurments()
{
}

float Measurments::distance(Point p1, Point p2)
{
    return sqrt(pow((p1.getX()-p2.getX()),2) + pow((p2.getY()-p2.getY()), 2));
}
