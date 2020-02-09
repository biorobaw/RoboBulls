#ifndef SECTOR_H
#define SECTOR_H
#include "region.h"
#include "vector"

#include "src/utilities/measurements.h"

/*! @file
 * This is a sector specialization of the abstract class, Region.
 * It can be used to define a circle with the appropriate parameters.
 */
class Sector : public Region
{
public:
    Sector();

    /*! A Sector is extended counter-clockwise from start_angle to end_angle.
     * @param Point center: Centre of the sector.
     * @param float radius: Radius of the sector.
     * @param float start_angle: Starting angle of sector in radians.
     * @param float end_angle: Ending angle of sector in radians.
     */
    Sector(Point centre, float radius, float start_angle, float end_angle);

    /*! @see Region::contains() */
    bool contains(const Point &) override;

    /*! @see Region::draw() */
    void draw() override;

    Point c;
    float r, a1, a2;
};

#endif // SECTOR_H
