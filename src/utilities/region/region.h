#ifndef REGION_H
#define REGION_H
#include "utilities/point.h"
#include "gui/gui_interface.h"

/*! @file
 * Region is an abstract class to define a 2D region on the field.
 * Regions expose some useful information querying functions once
 * the region is defined.
 */

class Region
{
public:
    /*! Returns true if the region contains the given point. */
    virtual bool contains(const Point&) = 0;

    /*! Draws the region on the GUI */
    virtual void draw() = 0;
};

#endif // REGION_H


