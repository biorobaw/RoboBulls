#ifndef FPPA_PATHFINDING_H
#define FPPA_PATHFINDING_H

/* Fast Path Planning Algorithm
 * Contributor: James
 *
 * This namespace implements the Fast Path Planning Algorithm (FPPA)
 * as described in the paper "Fast Path Planning Algorithm for the RoboCub Small Size League"
 * by Rodriguez S., Rojas E., Calderon J. et al.
 *
 * The algorithm is meant to provide reliable path-finding in real time, such as in
 * the case of the RoboCup league.
 */
#include <vector>
#include "utilities/point.h"

namespace FPPA
{
    enum class PathDirection
    {
        None, Bottom, Top
    };

    typedef std::vector<Point> Path;
    typedef std::pair<Path, PathDirection> PathInfo;


    /* Use FPPA to find a path from start to end */
    PathInfo findShortestPath(Point start, Point end, PathDirection pathHint = PathDirection::None);


    /* Check if there is an obstacle (robot or ball) in the
     * line defined from start to end*/
    bool isObstacleInLine(Point start, Point end, Point *obsPosOut = nullptr);


    /* Test if a given single point in question
     * is an obstacle in a given line */
    bool isPointInLine(Point start, Point end, Point question);


    /* Return a vector of all current obstacles
     * in the field, relitive to a starting and ending point. */
    void getCurrentObstacles(std::vector<Point>* points, Point start, Point end);
}

#endif // FPPA_PATHFINDING_H
