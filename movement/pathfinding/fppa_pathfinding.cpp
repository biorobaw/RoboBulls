#include <math.h>
#include <algorithm>
#include <utility>
#include <iostream>
#include <functional>
#include "include/config/tolerances.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "movement/pathfinding/fppa_pathfinding.h"

/* FPPA Pathfinding Constants */
#define FPPA_DEBUG 0
#define MAX_RECURSION_DEPTH 3
#define FRAME_UPDATE_COUNT 10

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
    int framesUntilUpdate = 0;
    std::vector<Point> currentFrameObstacles;

    bool insideRadiusRectangle(const Point& p0, const Point& p1, const Point& p2)
    {
        float rectTop = std::min(p1.y, p2.y) - ROBOT_RADIUS;
        float rectBottom = std::max(p1.y, p2.y) + ROBOT_RADIUS;
        float rectLeft = std::min(p1.x, p2.x) - ROBOT_RADIUS;
        float rectRight = std::max(p1.x, p2.x) + ROBOT_RADIUS;

        return (p0.x > rectLeft && p0.x < rectRight)
            && (p0.y > rectTop  && p0.y < rectBottom);
    }

    /*********************************************************/

    std::pair<bool, Point>
    isObstacleinLine(const Point& beginPos, const Point& endPos)
    {
        bool  obstacle_found = false;
        Point obstacle_position;

        for(const Point& pt : currentFrameObstacles)
        {
            /* With the new architecture, we need to manually exclude points that are
             * close to the start or ending point in this function.
             */
            if(Measurments::isClose(pt, beginPos, ROBOT_SIZE)
            || Measurments::isClose(pt, endPos, ROBOT_SIZE))
                continue;

            obstacle_found = Measurments::lineDistance(pt, beginPos, endPos) < ROBOT_SIZE &&
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


    bool isObstacleAtPoint(const Point& toCheck)
    {
        bool obstacle_found = false;

        obstacle_found = std::any_of
            (currentFrameObstacles.begin(), currentFrameObstacles.end(),
            [&](const Point& pt){return Measurments::isClose(pt, toCheck, ROBOT_SIZE);});

        return obstacle_found;
    }

    /*********************************************************/

    Point findSearchDirection(const Point& initialPos, const Point& endPos, int sign)
    {
        float theta = Measurments::angleBetween(initialPos, endPos);

        /* Reducing the search size by a factor can help eliminate
         * the jagged edges in the path, but risks cutting corners
         * too close around obstacles.
         */
        float dx = 2.25 * ROBOT_RADIUS * cos(theta + M_PI_2);
        float dy = 2.25 * ROBOT_RADIUS * sin(theta + M_PI_2);

        return Point(sign * dx, sign * dy);
    }

    /*********************************************************/

    void buildPathimpl(Path* results, const Point& beginPos, const Point& endPos,
                        int sign, int depth)
    {
        if(depth >= MAX_RECURSION_DEPTH)
            return;

        auto obstacle_info = isObstacleinLine(beginPos, endPos);

        if(obstacle_info.first == true)  //Obstacle was found
        {
            Point deltaPoint = findSearchDirection(beginPos, endPos, sign);
            Point subGoal;
            float i = 1;

            while(obstacle_info.first && (i < 10)) {
                subGoal = obstacle_info.second + Point(i * deltaPoint.x,  i * deltaPoint.y);
                obstacle_info.first = isObstacleAtPoint(subGoal);
                i += 0.5;
            }

            /* Find the ending point and insert the new point before it
             * (Possibly update this knowing the pos of the point)
             */
            auto it = std::find(results->begin(), results->end(), endPos);
            results->insert(it, subGoal);

            /* Recursively test the paths from the beginning to the subGoal and from
             * the subGoal to the end
             */
            buildPathimpl(results, beginPos, subGoal, sign, depth + 1);
            buildPathimpl(results, subGoal,  endPos, sign, depth + 1);
        }
    }

    /*********************************************************/

    /* This is the function that communicates outside the FPPA namespace to define
     * obstacle information. In our case, that is getting the GameModel and all the robots
     * from that. However, I would like to have this more generalized in the future.
     */
    void buildObstacleCollection()
    {
        GameModel* mod = GameModel::getModel();
        const auto& myTeam = mod->getMyTeam();
        const auto& opTeam = mod->getOponentTeam();

        currentFrameObstacles.clear();
        currentFrameObstacles.reserve(myTeam.size() + opTeam.size() + 1);

        for(Robot* rob : myTeam)
            currentFrameObstacles.push_back(rob->getRobotPosition());
        for(Robot* rob : opTeam)
            currentFrameObstacles.push_back(rob->getRobotPosition());

        currentFrameObstacles.push_back(mod->getBallPoint());

    #if FPPA_DEBUG
           // std::cout << "[FPPA] All Obstacles: " << std::endl;
            //for(Point pt : *obstacles) std::cout << pt.toString() << std::endl;
    #endif
    }

    /*********************************************************/

    std::pair<Path, Path> findBothPaths(const Point& start, const Point& end)
    {
        Path topPath, bottomPath;

        topPath.push_back(start);
        topPath.push_back(end);
        bottomPath.push_back(start);
        bottomPath.push_back(end);

        impl::buildPathimpl(&topPath, start, end, -1, 0);
        impl::buildPathimpl(&bottomPath, start, end,  1, 0);

        return std::make_pair(topPath, bottomPath);
    }

    /*********************************************************/

    bool isValidPath(const Path& p)
    {
        /* Bound function to determine if a point is inside the field or not */
        auto isPointInsideField =
            std::bind(insideRadiusRectangle, _1, fieldBotRight, fieldTopLeft);
        return std::all_of(p.begin(), p.end(), isPointInsideField);
    }


} //namespace impl


    void pathfindingBegin()
    {
        if(impl::framesUntilUpdate == 0) {
            impl::buildObstacleCollection();
            impl::framesUntilUpdate = FRAME_UPDATE_COUNT;
        }
    }


    void pathfindingEnd()
    {
        --impl::framesUntilUpdate;
    }


    PathInfo findShortestPath(const Point& start, const Point& end, PathDirection pathHint)
    {
        std::pair<Path, Path> foundPaths = impl::findBothPaths(start, end);

    #if FPPA_DEBUG
        std::cout << "Finding Path" << std::endl;
    #endif

        PathInfo topPath
            = std::make_pair(foundPaths.first, PathDirection::Top);
        PathInfo botPath
            = std::make_pair(foundPaths.second, PathDirection::Bottom);

        /* If one of the paths is invalid (contains points outside the field),
         * return the other path. Otherwise determine shortest. If both paths
         * are invalid, it still returns an invalid path,
         * but I am certain this never happens.
         */
        if(!impl::isValidPath(topPath.first)) {
            return botPath;
        } else if(!impl::isValidPath(botPath.first)) {
            return topPath;
        }

        if(pathHint != PathDirection::None) {
            /* The user has requested a certain direction of path
             * be chosen.
             */
            if(pathHint == PathDirection::Top)
               return topPath;
            else
               return botPath;
         } else {
            /* Add up all the distances from each path and determine which
             * path is shorter. This shorter path is deemed the "better" path.
             */
            float totalDistTop = 0, totalDistBottom = 0;

            for(auto it = topPath.first.begin();
                     it != topPath.first.end()-1; ++it)
                totalDistTop += Measurments::distance(*it, *(it+1));

            for(auto it = botPath.first.begin();
                     it != botPath.first.end()-1; ++it)
                totalDistBottom += Measurments::distance(*it, *(it+1));

            if(totalDistTop < totalDistBottom) {
                return topPath;
            } else {
                return botPath;
            }
        }
    }


    bool isObstacleInLine(const Point& start, const Point& end, Point* obsPosOut)
    {
        auto obstacle_info = impl::isObstacleinLine(start, end);

        if(obstacle_info.first) {
            if(obsPosOut != nullptr) *obsPosOut = obstacle_info.second;
            return Measurments::isClose(start, obstacle_info.second, 2500);
        } else {
            return false;
        }
    }


    bool isPointInLine(const Point& start, const Point& end, const Point& question)
    {
        return Measurments::lineDistance(question, start, end) < ROBOT_SIZE &&
               impl::insideRadiusRectangle(question, start, end);
    }


    const std::vector<Point>& getCurrentObstacles()
    {
        return impl::currentFrameObstacles;
    }


    //std::vector<Point> getObstaclesForRobot(int id)
    //{
    //    UNUSED_PARAM(id);
    //}

} //namespace FPPA
