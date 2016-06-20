#ifndef DEFENCEAREA_H
#define DEFENCEAREA_H

#include "include/config/globals.h"
#include "rectangle.h"
#include "sector.h"
#include "iostream"
#include "include/config/team.h"


class DefenceArea : public Region
{
public:
    DefenceArea(bool team = !config->our_team);
    bool contains(const Point &) override;
    bool contains(const Point &, const float tolerance);
    void draw() override;
    void expelPoint(Point&);
    std::vector<Point> lineIntercepts(const Point& A, const Point& B);
private:
    // Consult SSL Rule book for clarification of the following regions
    Rectangle r;
    Sector s1, s2;
    bool team;
};

#endif // DEFENCEAREA_H
