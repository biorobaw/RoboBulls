#define _USE_MATH_DEFINES
#include <cmath>
#include "sector.h"
#include <iostream>

Sector::Sector(){}

Sector::Sector(Point centre, float radius, float start_angle, float end_angle)
    :c(centre), r(radius), a1(start_angle), a2(end_angle)
{
}

bool Sector::contains(const Point& p)
{
    // Check if within radius
    if(Measurements::distance(c, p) > r)
        return false;

    // Get angle to point
    double ap = Measurements::angleBetween(c, p);

    // Convert Angles
    while(a1 < 0)
        a1 += 2*M_PI;

    while(a2 < 0 || a1 > a2)
        a2 += 2*M_PI;

    if(ap < 0)
        ap += 2*M_PI;

    // Check if within angle
    if(ap >= a1 && ap <= a2)
        return true;
    return false;
}

void Sector::draw()
{
    for(int x = c.x - r; x <= c.x + r; x+=10)
        for(int y = c.y - r; y <= c.y + r; y+=10)
            if(this->contains(Point(x,y)))
                GuiInterface::getGuiInterface()->drawPoint(Point(x,y));
}
