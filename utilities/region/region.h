#ifndef REGION_H
#define REGION_H
#include "utilities/point.h"
#include "gui/guiinterface.h"

/*! @brief `Region` is an abstract utility class to define a 2D region on the field.
 * @author Muhaimen Shamshi
 * Regions expose some useful information querying functions once
 * the region is defined. */

class Region
{
public:
    virtual bool contains(const Point&) = 0;
    virtual void draw() = 0;
};

#endif // REGION_H


