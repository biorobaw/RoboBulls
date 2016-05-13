#include "defencearea.h"

DefenceArea::DefenceArea(bool team)
{
    if(team == OUR_TEAM)    // Initialize our defence area
    {
        r = Rectangle(-HALF_FIELD_LENGTH,
                        -DEF_AREA_OFFSET,
                        -HALF_FIELD_LENGTH + DEF_AREA_RADIUS,
                        DEF_AREA_OFFSET);

        s1 = Sector(Point(-HALF_FIELD_LENGTH, DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            0, M_PI_2);

        s2 = Sector(Point(-HALF_FIELD_LENGTH, -DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            -M_PI_2, 0);
    }
    else    // initialize opponent defence area
    {
        r = Rectangle(HALF_FIELD_LENGTH,
                        -DEF_AREA_OFFSET,
                        HALF_FIELD_LENGTH - DEF_AREA_RADIUS,
                        DEF_AREA_OFFSET);

        s1 = Sector(Point(HALF_FIELD_LENGTH, DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            M_PI_2, M_PI);

        s2 = Sector(Point(HALF_FIELD_LENGTH, -DEF_AREA_OFFSET),
                            DEF_AREA_RADIUS,
                            -M_PI, -M_PI_2);
    }
}

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
