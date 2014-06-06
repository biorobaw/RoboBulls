#ifndef MEASURMENTS_H
#define MEASURMENTS_H

#include <math.h>
#include <algorithm>
#include "utilities/point.h"
#include "include/globals.h"

class Measurments
{
public:
    Measurments(){}


    /* Returns the distance between two points using the
     * standard distance formula.
     */
    static float distance(const Point&, const Point&);


    /* Returns the angle between two points as measured
     * from the horizontal.
     */
    static float angleBetween(const Point&, const Point&);


    /* Compare the points to determine if they are within a certain
     * tolerance of each other. A "close enough" alternitive to
     * the == operator.
     */
    static bool isClose(const Point&, const Point&, float tol = DIST_TOLERANCE);


    /* Given a container of points, returns the closest point in the
     * container to p0.
     */
    template <class Container>
    static Point closestPoint(const Container& cntr, Point p0);


    //Calculates the slope, given two points
    static float slope(Point, Point);


    // Calculates the smallest difference between two orientations (angle2 - angle1)
    static float angleDiff(float angle1, float angle2);


    // Calculates the sum of two orientations (angle2 + angle1)
    static float angleSum(float angle1, float angle2);
};


template <class Container>
Point Measurments::closestPoint(const Container& cntr, Point p0)
{
    auto pos = std::min_element(cntr.begin(), cntr.end(),
        [&](Point a, Point b) {
			return Measurments::distance(p0, a) < Measurments::distance(p0, b);
		});
    return *pos;
}


#endif // MEASURMENTS_H
