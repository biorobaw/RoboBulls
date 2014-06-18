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


    /* Given a container of points, returns an iterator to the closest point 
     * in the container to p0. 'cntr' can be of any type, including plain-old
	 * C-style arrays or STL containers.
     */
    template <class Container>
    static auto closestPoint(Container& cntr, const Point p0) -> decltype(std::begin(cntr));


    //Calculates the slope, given two points
    static float slope(Point, Point);


    // Calculates the smallest difference between two orientations (angle2 - angle1)
    static float angleDiff(float angle1, float angle2);


    // Calculates the sum of two orientations (angle2 + angle1)
    static float angleSum(float angle1, float angle2);
	
	
	/* Clamps a value beteen min and max */
	template<typename T>
    static T clamp(const T& value, const T& min, const T& max);
};


/***************************************************/


template <class Container>
auto Measurments::closestPoint(Container& cntr, const Point p0) -> decltype(std::begin(cntr))
{
	auto pos = std::min_element(std::begin(cntr), std::end(cntr),
		[&](Point a, Point b) {
			return Measurments::distance(p0, a) < Measurments::distance(p0, b);
		});
	return pos;
}


template<typename T> 
T Measurments::clamp(const T& value, const T& min, const T& max)
{
    if(value > max) return max;
    if(value < min) return min;

    return value;
    //return std::min(max, std::max(min, value));
    //return std::max(min, std::min(max, value));
}


#endif // MEASURMENTS_H
