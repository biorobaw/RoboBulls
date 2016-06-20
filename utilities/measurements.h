#ifndef measurements_H
#define measurements_H

#include <cmath>
#include <vector>
#include "utilities/point.h"
#include "include/config/tolerances.h"

class Robot;

/*! @addtogroup everydayuse
 * @{ */

/*! @brief Contains useful math and utility functions
 * @details measurements is used over the project to calculate various
 * distances and angles. It’s also misspelled but left that way for
 * backwards compatibility. Many of these functions (distance, midpoint, angleBetween)
 * have four Robot* / Point combination overloads. */

class Measurements
{
public:
    /*! @name 2D Distance Formula Family
     * @{
     * @brief Returns the distance between two points using the
     * standard distance formula. */
    static float mag(const Point&);
    static float distance(const Point&, const Point&);
    static float distance(const Point&, Robot*);
    static float distance(Robot*, const Point&);
    static float distance(Robot*, Robot*);

    /*! @brief Returns the midpoint between two points a and b using
     * the standard 2D midpoint formula */
    static Point midPoint(const Point&, const Point&);
    static Point midPoint(const Point&, Robot*);
    static Point midPoint(Robot*, const Point&);
    static Point midPoint(Robot*, Robot*);

    /*! @brief Compare the points to determine if they are within a certain
     * tolerance of each other. A "close enough" alternative to
     * the == operator. */
    static bool isClose(const Point&, const Point&, float tol = DIST_TOLERANCE);
    static bool isClose(const Point&, Robot*, float tol = DIST_TOLERANCE);
    static bool isClose(Robot*, const Point&, float tol = DIST_TOLERANCE);
    static bool isClose(Robot*, Robot*, float tol = DIST_TOLERANCE);
    //!@}


    /*! @name Angle functions family
     * @{
     * @brief Returns the angle between two points as measured
     * from the horizontal. */
    static float angleBetween(const Point&, const Point&);
    static float angleBetween(const Point&, Robot*);
    static float angleBetween(Robot*, const Point&);
    static float angleBetween(Robot*, Robot*);

    //!@brief Calculates the smallest difference between two orientations (angle2 - angle1)
    static float angleDiff(float angle1, float angle2);

    //!@brief Calculates the sum of two orientations (angle2 + angle1) */
    static float angleSum(float angle1, float angle2);

    /*! @brief isClose overload for angles
     *  @see isClose */
    static bool isClose(float angle1, float angle2, float tol = ROT_TOLERANCE);
    //!@}

    //!@brief returns true if an angle lies within
    static bool angleInRange(float angle, float start, float end);

    /*! @name Line Functions Family
     * @{
     * @brief Given a line defined by LStart and LEnd, returns the shortest distance from the
	 * line to the point p0; this is always the length of the line perpendicular to
     * LStart and LEnd touching p0. */
	static float lineDistance(const Point& p0, const Point& LStart, const Point& LEnd);

    /*! @brief Given a line A defined by LStart and LEnd, returns the point on A that is
     * closest to p0. Similar to above. */
    static Point lineSegmentPoint(const Point& p0, const Point& LStart, const Point& LEnd);

    /*! @brief Given a line segment defined by LStart and LEnd, returns the shortest distance from the
    * line segment to the point p0. */
   static float lineSegmentDistance(const Point& p0, const Point& LStart, const Point& LEnd);

    /*! @brief Calculates the slope, given two points */
    static float slope(Point, Point);
    //! @}

    /*! @brief Returns true if an obstacle-free path is found between two points */
    static bool noObstaclesInPath(const std::vector<Point>& obstacles, const Point& A, const Point& B, const int& tolerance);
    static Robot* robotInPath(const std::vector<Robot*>& obstacles, const Point& A, const Point& B, const int& tolerance);
    //! @}

    //!@brief Clamps (limits) a value between min and max
    template<typename T>
    static T clamp(const T& value, const T& min, const T& max);

    //!@brief Returns a unit vector
    static Point unitVector(const Point&);
};

//! @}

/***************************************************/

template<typename T> 
T Measurements::clamp(const T& value, const T& min, const T& max) {
    return std::min(max, std::max(value, min));
}

#endif // measurements_H
