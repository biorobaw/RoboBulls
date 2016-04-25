#include "defencearea.h"

Rectangle DefenceArea::r = Rectangle(-HALF_FIELD_LENGTH,
                       -DEF_AREA_OFFSET,
                        -HALF_FIELD_LENGTH + DEF_AREA_RADIUS,
                        DEF_AREA_OFFSET);

Sector DefenceArea::s1 = Sector(Point(-HALF_FIELD_LENGTH, DEF_AREA_OFFSET),
                       DEF_AREA_RADIUS,
                       M_PI_2, 0);

Sector DefenceArea::s2 = Sector(Point(-HALF_FIELD_LENGTH, -DEF_AREA_OFFSET),
                       DEF_AREA_RADIUS,
                       0, -M_PI_2);

DefenceArea::DefenceArea(){}

bool DefenceArea::contains(const Point& p)
{
    return r.contains(p) || s1.contains(p) || s2.contains(p);
}

void DefenceArea::draw()
{
    r.draw();
    s1.draw();
    s2.draw();
}
