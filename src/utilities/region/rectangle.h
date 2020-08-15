#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "region.h"

#include <sstream>
#include <cmath>
#include <vector>
#include <set>

class Robot;

/*! @file
 * This is a rectangular specialization of the abstract class Region.
 */

class Rectangle : public Region
{
public:
    Rectangle();

    /*! Constructs rectangle with a bottom-left point (X1,Y1) and
     *  top-right point (X2,Y2)
     */
    Rectangle(float X1, float Y1, float X2, float Y2);

    /*! Returns true if a point is inside the rectangular region */
    bool contains(const Point& point) override;

    /*! Displays the region on the GUI using a point cloud*/
    void draw() override;

    /*! Returns true if the line from p0 and p1 passes through the region */
    bool containsLine(const Point& p0, const Point& p1);

    /*! Returns the number of robots in the set that are within the region */
    int numOfRobots(std::set<Robot*>& robots);

    /*! cohen sutherland algorithm for clipping a line with a rectangle
        source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
    */
    std::vector<Point> intersectSegment(Point PA, Point PB);

    /* algorithm that returns the intersection of the line with the rectangle */
    std::vector<Point> intersectLine(Point PA, Point PB);


    /*! Returns the calculated center of the Region */
    Point centre();

    /*! Returns a string representation of the Region */
    std::string toString();

    float min_x, max_x, min_y, max_y;

};

#endif // RECTANGLE_H
