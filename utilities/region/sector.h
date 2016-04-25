#ifndef SECTOR_H
#define SECTOR_H
#include "region.h"
#include "vector"

#include "utilities/measurements.h"

class Sector : public Region
{
public:
    // Sector is extended clockwise from start_angle to end_angle
    Sector(Point centre, float radius, float start_angle, float end_angle);
    bool contains(const Point &) override;
    void draw() override;
private:
    Point c;
    float r, a1, a2;
};

#endif // SECTOR_H
