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
#include "include/config/tolerances.h"

/* FPPA Pathfinding Constants */
#define FPPA_DEBUG 0
#define MAX_RECURSION_DEPTH  3
#define ROBOT_SIZE (ROBOT_RADIUS * 2.5)
#define FRAME_UPDATE_COUNT 8

namespace FPPA
{
    enum class PathDirection
    {
        None, Bottom, Top
    };

    typedef std::vector<Point> Path;
    typedef std::pair<Path, PathDirection> PathInfo;

    /****************************************************************/

    /* Call these at the beginning and end of each frame in StrategyController.
     * PathfindingBegin calculates the obstacles for the current frame, and
     * PathfindingEnd tells FPPA that the frame has ended, and a new obstacle
     * collection is needed.
     */
    void pathfindingBegin();
    void pathfindingEnd();

    /****************************************************************/

    /* Use FPPA to find a path from start to end. If a path hint is provided,
     * The algorithm will prioritize returning the path as either the top or
     * bottom path, whatever is specified. Otherwise, finds shortest.
     * The returned PathDirection (PathInfo.second) indicates whether this path 
     * is a top path, or bottom path.
     */
    PathInfo findShortestPath(const Point& start, const Point& end,
                              PathDirection pathHint = PathDirection::None);


    /* Check if there is an obstacle (robot or ball) in the
     * line defined from start to end. if obsPosOut is not a nullptr, it will be filled
     * with the found obstacle position if this function returns true.
     */
    bool isObstacleInLine(const Point& start, const Point& end, Point *obsPosOut = nullptr);


    /* Test if a given single point in question
     * is an obstacle in a given line 
     */
    bool isPointInLine(const Point& start, const Point& end, const Point& question);


    /* Returns a reference to the vector consisting of the positions of
     * all obstacles for this frame 
     */
    const std::vector<Point>& getCurrentObstacles();


    /* Returns a vector of the current obstacles (returned by getCurrentObstacles)
     * sorted by distance to robot with the id specified. 
     */
    //std::vector<Point> getObstaclesForRobot(int id);
}

#endif // FPPA_PATHFINDING_H
