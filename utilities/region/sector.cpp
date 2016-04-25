#include "sector.h"

Sector::Sector(Point centre, float radius, float start_angle, float end_angle)
    :c(centre), r(radius), a1(start_angle), a2(end_angle)
{
}

bool Sector::contains(const Point& p)
{
    // Check if within radius
    if(Measurements::distance(c, p) > r)
        return false;

    // Check if within angle
    if(a1 >= 0 && a2 >= 0)
    {
        if(Measurements::angleBetween(c, p) > a1
        || Measurements::angleBetween(c, p) < a2)
            return false;
    }
    else if(a1 >= 0 && a2 <= 0)
    {
        if(Measurements::angleBetween(c, p) > a1
        || Measurements::angleBetween(c, p) < a2)
            return false;
    }
    if(a1 <= 0 && a2 >= 0)
    {
        if(Measurements::angleBetween(c, p) < a1
        || Measurements::angleBetween(c, p) > a2)
            return false;
    }
    if(a1 <= 0 && a2 <= 0)
    {
        if(Measurements::angleBetween(c, p) > a1
        || Measurements::angleBetween(c, p) < a2)
            return false;
    }

    return true;
}

void Sector::draw()
{
    std::vector<Point> sector;
    for(int x = c.x - r; x <= c.x + r; x+=20)
        for(int y = c.y - r; y <= c.y + r; y+=20)
            if(this->contains(Point(x,y)))
                sector.push_back(Point(x,y));

    GuiInterface* gui = GuiInterface::getGuiInterface();
    gui->drawRegion(sector);
}
