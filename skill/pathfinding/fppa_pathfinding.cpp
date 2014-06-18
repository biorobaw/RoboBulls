#include <math.h>
#include <algorithm>
#include <utility>
#include <iostream>
#include <functional>
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "include/globals.h"

#include "skill/pathfinding/fppa_pathfinding.h"

/* Implementation of the Fast Path Planning Algorithm
 * In a sense, this is a mostly a generalized implementation.
 * But it is currently hardcoded to work with robobulls only.
 * JamesW.
 */
using namespace std::placeholders;

namespace FPPA
{
namespace impl
{
    const Point fieldTopLeft = Point(-3000, -2000);
    const Point fieldBotRight = Point(3000, 2000);

    float perpendicularDistance(const Point& p0, const Point& LStart, const Point& LEnd)
    {
        float y2 = LEnd.y;
        float y1 = LStart.y;
        float x2 = LEnd.x;
        float x1 = LStart.x;

        return fabs((y2-y1)*p0.x - (x2-x1)*p0.y - x1*y2 + x2*y1)
                / Measurments::distance(LStart, LEnd);
    }

    bool insideRadiusRectangle(const Point& p0, const Point& p1, const Point& p2)
    {
        float rectTop = std::min(p1.y, p2.y) - ROBOT_SIZE;
        float rectBottom = std::max(p1.y, p2.y) + ROBOT_SIZE;
        float rectLeft = std::min(p1.x, p2.x) - ROBOT_SIZE;
        float rectRight = std::max(p1.x, p2.x) + ROBOT_SIZE;

        return (p0.x > rectLeft && p0.x < rectRight) && (p0.y > rectTop && p0.y < rectBottom);
    }

    /*********************************************************/

    std::pair<bool, Point>
    isObstacleinLine(std::vector<Point>* obstacles, Point& beginPos, Point& endPos)
    {
        bool  obstacle_found = false;
        Point obstacle_position;

        for(Point& pt : *obstacles)
        {
            obstacle_found = perpendicularDistance(pt, beginPos, endPos) < ROBOT_SIZE &&
                insideRadiusRectangle(pt, beginPos, endPos);

            if(obstacle_found) {
                obstacle_position = pt;
                break;
            }
        }

    #if FPPA_DEBUG
        //printf("[FPPA] Checking Line (%f,%f)-(%f,%f)\n", beginPos.x, beginPos.y, endPos.x, endPos.y);
        if(obstacle_found)
            printf("[FPPA] Found: %.0f,%.0f\n", obstacle_position.x, obstacle_position.y);
    #endif

        return std::pair<bool, Point>(obstacle_found, obstacle_position);
    }


    bool isObstacleAtPoint(std::vector<Point>* obstacles, Point& toCheck)
    {
        bool obstacle_found = false;

        obstacle_found = std::any_of(obstacles->begin(), obstacles->end(),
            [&](Point pt){return Measurments::isClose(pt, toCheck, ROBOT_SIZE);});

        return obstacle_found;
    }

    /*********************************************************/

    Point findSearchDirection(Point& initialPos, Point& endPos, int sign)
    {
        float theta = Measurments::angleBetween(initialPos, endPos);

        /* Reducing the search size by a fisPointInsideFieldactor can help eliminate
         * the jagged edges in the path, but risks cutting corners
         * too close around obstacles.
         */
        float dx = ROBOT_SIZE * cos(theta + M_PI_2);
        float dy = ROBOT_SIZE * sin(theta + M_PI_2);

        return Point(sign * dx, sign * dy);
    }

    /*********************************************************/

    void buildPathimpl(Path* results, std::vector<Point>* obstacles, Point beginPos, Point endPos,
                       int sign, int depth)
    {
        if(depth >= MAX_RECURSION_DEPTH)
            return;

        auto obstacle_info = isObstacleinLine(obstacles, beginPos, endPos);

        if(obstacle_info.first == true)  //Obstacle was found
        {
            Point deltaPoint = findSearchDirection(beginPos, endPos, sign);
            Point subGoal;
            int i = 1;

            while(obstacle_info.first && (i < 10)) {
                subGoal = obstacle_info.second + Point(i * deltaPoint.x,  i * deltaPoint.y);
                obstacle_info.first = isObstacleAtPoint(obstacles, subGoal);
                i += 1;
            }

            /* Find the ending point and insert the new point before it
             * (Possibly update this knowing the pos of the point)
             */
            auto it = std::find(results->begin(), results->end(), endPos);
            results->insert(it, subGoal);

            /* Recursively test the paths from the beginning to the subGoal and from
             * the subGoal to the end
             */
            buildPathimpl(results, obstacles, beginPos, subGoal, sign, depth + 1);
            buildPathimpl(results, obstacles,  subGoal,  endPos, sign, depth + 1);
        }
    }

    /*********************************************************/

    /* This is the function that communicates outisde the FPPA namespace to define
     * obstacle information. In our case, that is getting the GameModel and all the robots
     * from that. However, I would like to have this more generalized in the future.
     */
    void buildObstacleCollection(std::vector<Point>* obstacles, Point start, Point end)
    {
        GameModel* mod = GameModel::getModel();

        obstacles->reserve(mod->getMyTeam().size() + mod->getOponentTeam().size() + 1);

        obstacles->push_back(mod->getBallPoint());

        for(Robot* rob : mod->getMyTeam())
            obstacles->push_back(rob->getRobotPosition());
        for(Robot* rob : mod->getOponentTeam())
            obstacles->push_back(rob->getRobotPosition());

        /* Remove obstacles that are close to the start and the end.
         * This eliminates the algorithm not being able to reach the target
         * because there is an obstacle right next to the target.
         */
        auto last_pos = std::remove_if(obstacles->begin(), obstacles->end(),
            [&](Point pt) {
                return Measurments::isClose(pt, start, ROBOT_SIZE) ||
                       Measurments::isClose(pt, end, ROBOT_SIZE);
            });

        obstacles->erase(last_pos, obstacles->end());

        /* Sort the range on how close each point is to the start.
         * May help with forming proper paths by always finding the
         * closest obstacle first.
         */
        std::sort(obstacles->begin(), obstacles->end(),
            [&](Point a, Point b) {
                return Measurments::distance(start, a) < Measurments::distance(start, b);
            });

    #if FPPA_DEBUG
           // std::cout << "[FPPA] All Obstacles: " << std::endl;
            //for(Point pt : *obstacles) std::cout << pt.toString() << std::endl;
    #endif
    }


    std::pair<Path, Path> findBothPaths(Point& start, Point& end)
    {
        Path topPath, bottomPath;
        std::vector<Point> obstacles;

        topPath.push_back(start);
        topPath.push_back(end);
        bottomPath.push_back(start);
        bottomPath.push_back(end);

        impl::buildObstacleCollection(&obstacles, start, end);

        impl::buildPathimpl(&topPath, &obstacles, start, end, -1, 0);
        impl::buildPathimpl(&bottomPath, &obstacles, start, end,  1, 0);

        return std::pair<Path, Path>(topPath, bottomPath);
    }

    /*********************************************************/

    bool isValidPath(const Path& p)
    {
        /* Bound function to determine if a point is inside the field or not */
        static auto isPointInsideField =
            std::bind(insideRadiusRectangle, _1, fieldBotRight, fieldTopLeft);

        return std::all_of(p.begin(), p.end(), isPointInsideField);
    }


} //namespace impl


    Path findShortestPath(Point start, Point end)
    {
        float totalDistTop = 0, totalDistBottom = 0;

        std::pair<Path, Path> foundPaths = impl::findBothPaths(start, end);


        /* If one of the paths is invalid (contains points outside the field),
         * return the other path. Otherside determine shortest. If both paths
         * are invalid, it still returns an invalid path,
         * but I am certain this never happens.
         */
        if(!impl::isValidPath(foundPaths.first)) {
            return foundPaths.second;
        } else if(!impl::isValidPath(foundPaths.second)) {
            return foundPaths.first;
        }

        /* Add up all the distances from each path and determine which path is shorter.
         * This shorter path is deemed the "better" path
         */
        for(auto it = foundPaths.first.begin(); it != foundPaths.first.end()-1; ++it)
            totalDistTop += Measurments::distance(*it, *(it+1));

        for(auto it = foundPaths.second.begin(); it != foundPaths.second.end()-1; ++it)
            totalDistBottom += Measurments::distance(*it, *(it+1));

        if(totalDistTop <= totalDistBottom) {
            return foundPaths.first;
        } else {
            return foundPaths.second;
        }
    }

    bool isObstacleInLine(Point start, Point end)
    {
        std::vector<Point> obstacles;

        impl::buildObstacleCollection(&obstacles, start, end);
        auto obstacle_info = impl::isObstacleinLine(&obstacles, start, end);

        return obstacle_info.first;
    }

} //namespace FPPA
