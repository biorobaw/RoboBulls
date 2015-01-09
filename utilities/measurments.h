#ifndef MEASURMENTS_H
#define MEASURMENTS_H

#include <cmath>
#include "utilities/point.h"
#include "include/config/tolerances.h"

class Robot;

class Measurments
{
public:
    Measurments(){}

    /* Returns the distance between two points using the
     * standard distance formula.
     */
    static float distance(const Point&, const Point&);
    static float distance(const Point&, Robot*);
    static float distance(Robot*, const Point&);
    static float distance(Robot*, Robot*);


    /* Returns the midpoint between two points a and b using
     * the standard 2D midpoint formula
     */
    static Point midPoint(const Point&, const Point&);
    static Point midPoint(const Point&, Robot*);
    static Point midPoint(Robot*, const Point&);
    static Point midPoint(Robot*, Robot*);

    /* Returns the angle between two points as measured
     * from the horizontal.
     */
    static float angleBetween(const Point&, const Point&);
    static float angleBetween(const Point&, Robot*);
    static float angleBetween(Robot*, const Point&);
    static float angleBetween(Robot*, Robot*);

    /* Compare the points to determine if they are within a certain
     * tolerance of each other. A "close enough" alternative to
     * the == operator.
     */
    static bool isClose(const Point&, const Point&, float tol = DIST_TOLERANCE);
    static bool isClose(const Point&, Robot*, float tol = DIST_TOLERANCE);
    static bool isClose(Robot*, const Point&, float tol = DIST_TOLERANCE);
    static bool isClose(Robot*, Robot*, float tol = DIST_TOLERANCE);


    //Calculates the slope, given two points
    static float slope(Point, Point);


    // Calculates the smallest difference between two orientations (angle2 - angle1)
    static float angleDiff(float angle1, float angle2);


    // Calculates the sum of two orientations (angle2 + angle1)
    static float angleSum(float angle1, float angle2);
	

	/* isClose overload for angles; wrapper around angleDiff to make doing this less
	 * of a pain.
	 */
    static bool isClose(float angle1, float angle2, float tol = ROT_TOLERANCE);
	

	/* Clamps a value between min and max */
	template<typename T>
    static T clamp(const T& value, const T& min, const T& max);
	
	
	/* Given a line defined by LStart and LEnd, returns the shortest distance from the 
	 * line to the point p0; this is always the length of the line perpendicular to
	 * LStart and LEnd touching p0.
	 */
	static float lineDistance(const Point& p0, const Point& LStart, const Point& LEnd);
};


/***************************************************/

template<typename T> 
T Measurments::clamp(const T& value, const T& min, const T& max) {
    return std::min(max, std::max(value, min));
}

#endif // MEASURMENTS_H
