#ifndef MEASURMENTS_H
#define MEASURMENTS_H

#include <math.h>
#include "Utilities/point.h"

class Measurments
{
public:
    Measurments(){}


    /*  Returns the distance between two points using the
        standard distance formula.
        */
    static float distance(const Point&, const Point&);


    /*  Returns the angle in (degrees)? between two points as measured
        from the horizontal.
        */
    static float angleBetween(const Point&, const Point&);


    /*  Compare the points to determine if they are within a certain
        tolerance of each other. A "close enough" alternitive to
        the == operator.
        */
    static bool isClose(const Point&, const Point&, float tol = 0.5f);


    /*	Return the point, and the index of the point, in the n-point array
        that is closest to the given point p1.
        */
    static int closestPoint(const Point&, const Point*, Point* out = NULL);
};

#endif // MEASURMENTS_H
