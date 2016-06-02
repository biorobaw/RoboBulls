#include <math.h>
#include <algorithm>
#include <iostream>
#include "include/config/tolerances.h"
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "utilities/region/defencearea.h"
#include "model/gamemodel.h"
#include "movement/pathfinding/fppa_pathfinding.h"

/* Implementation of the Fast Path Planning Algorithm
 * In a sense, this is a mostly a generalized implementation.
 * But it is currently hardcoded to work with robobulls only.
 * JamesW.
 */

/************************************************************************/
/* USER CONFIGURATION */

//Print some extra debug output
#define FPPA_DEBUG 0
/* Defines the maximum recursions buildPathimpl makes(the number of segments in a path)
 * before no more serch is made
 */
#define MAX_RECURSION_DEPTH 10
/* Defines the number of calls to pathfindingBegin (to update FPPA) must be made before
 * the state of all obstacles is updated. This is for efficiency reasons
 */
#define FRAME_UPDATE_COUNT 3

/* Defines, when sub-divding a path to avoid an obstalce, the minimum perpendicular distance
 * it samples from the segment to avoid it. Reducing the search size by a factor can
 * help eliminate the jagged edges in the path, but risks cutting corners too close around
 * obstacles.
 */
#define SRCH_INCR 20

/************************************************************************/

namespace Movement {
namespace FPPA {
namespace impl {

    int framesUntilUpdate = 0;
    std::vector<Point> currentRobotObstacles;

    static std::pair<bool, Point>
    isObstacleinLine(const Point& beginPos, const Point& endPos, bool avoidBall, bool use_def_areas)
    {
        bool  obstacle_found = false;
        Point obstacle_position;

        // Check ball
        if(avoidBall)
        {
            Point bp = gameModel->getBallPoint();
            obstacle_found = Measurements::lineSegmentDistance(bp, beginPos, endPos) <= ROBOT_RADIUS+BALL_RADIUS+10;

            if(obstacle_found)
                obstacle_position = bp;
        }

        // Check robots
        for(const Point& obstacle : currentRobotObstacles)
        {
            // Here we exclude obstacle points that are too close to the starting point,
            if(Measurements::isClose(obstacle, beginPos, ROBOT_RADIUS))
                continue;

            if(Measurements::lineSegmentDistance(obstacle, beginPos, endPos) <= ROBOT_RADIUS*2+10)
                obstacle_found = true;

            if(obstacle_found) {
                obstacle_position = obstacle;
                break;
            }
        }

        // Check defence areas
        if(use_def_areas)
        {
            std::vector<Point> intercepts;
            DefenceArea da0(0);
            intercepts = da0.lineIntercepts(beginPos, endPos);

            if(!intercepts.empty())
            {
                obstacle_found = true;
                obstacle_position = (intercepts.front() + intercepts.back())*0.5;
            }

            DefenceArea da1(1);
            intercepts = da1.lineIntercepts(beginPos, endPos);

            if(!intercepts.empty())
            {
                obstacle_found = true;
                obstacle_position = (intercepts.front() + intercepts.back())*0.5;
            }
        }

        return std::pair<bool, Point>(obstacle_found, obstacle_position);
    }


    static bool isObstacleAtPoint(const Point& toCheck, bool avoid_ball, bool use_def_areas)
    {
        // Check ball
        bool ball_in_the_way = false;
        if(avoid_ball)
            ball_in_the_way = Measurements::isClose(toCheck, gameModel->getBallPoint(), ROBOT_RADIUS+BALL_RADIUS + 50);

        // Check defence areas
        bool def_area_occupied = false;
        if(use_def_areas)
        {
            DefenceArea da0(OUR_TEAM), da1(!OUR_TEAM);
            def_area_occupied = da0.contains(toCheck, DEF_AREA_TOL) || da1.contains(toCheck, DEF_AREA_TOL);
        }

        // Check robots and return
        return Comparisons::isDistanceToLess(toCheck, ROBOT_RADIUS*2 + 50).any_of(currentRobotObstacles)
            || ball_in_the_way
            || def_area_occupied;
    }

    /*********************************************************/

    static Point findUnitOrth(const Point& initialPos, const Point& endPos)
    {
        /* The search point is `SEARCH_DIRECTION_DIST` points perpendicularly from
         * the line between initialPos and endPos, with sign determining the direction */
        float theta = Measurements::angleBetween(initialPos, endPos);
        float dx = cos(theta + M_PI_2);
        float dy = sin(theta + M_PI_2);
        return Point(dx,dy);
    }

    /*********************************************************/

    static void recursiveFPPA(Path* results, const Point& beginPos, const Point& endPos,
                              int depth, bool avoidBall, bool use_def_areas)
    {
        if(depth >= MAX_RECURSION_DEPTH)
            return;

        // Determine if the robot we are planning for is the goalie
        // and hence whether the def_areas are to be considered obstacles
        auto obstacle_info = isObstacleinLine(beginPos, endPos, avoidBall, use_def_areas);

        if(obstacle_info.first == true)  // Obstacle was found
        {
            //GuiInterface::getGuiInterface()->drawLine(beginPos, endPos, 0.01);
            Point nearest_to_obstacle = Measurements::lineSegmentPoint(obstacle_info.second, beginPos, endPos);

            Point srch_vctr = findUnitOrth(beginPos, endPos);
            bool subGoal0_obstructed = true, subGoal1_obstructed = true;
            Point subGoal0, subGoal1;

            // Check along one direction
            float i0 = 0;
            while(subGoal0_obstructed)
            {
                i0 += SRCH_INCR;
                subGoal0 = nearest_to_obstacle + (srch_vctr * i0);
                subGoal0_obstructed = isObstacleAtPoint(subGoal0, avoidBall, use_def_areas);
            }

            // Check along other direction
            float i1 = 0;
            while(subGoal1_obstructed)
            {
                i1 += SRCH_INCR;
                subGoal1 = nearest_to_obstacle - (srch_vctr * i1);
                subGoal1_obstructed = isObstacleAtPoint(subGoal1, avoidBall, use_def_areas);
            }

            // Choose less deviant sub_goal
            Point subGoal;
            if(i0 < i1)
                subGoal = subGoal0;
            else
                subGoal = subGoal1;

            // Find the ending point and insert the new point before it
            auto it = std::find(results->begin(), results->end(), endPos);
            results->insert(it, subGoal);

            // Recursively test the paths from the beginning to the subGoal and from
            // the subGoal to the end
            recursiveFPPA(results, beginPos, subGoal, depth + 1, avoidBall, use_def_areas);
            recursiveFPPA(results, subGoal,  endPos, depth + 1, avoidBall, use_def_areas);
        }
    }

    /*********************************************************/

    /* This is the function that communicates outside the FPPA namespace to define
     * obstacle information. In our case, that is getting the GameModel and all the robots
     * from that.
     */
    static void genRobotObstacles()
    {
        const auto& myTeam = gameModel->getMyTeam();
        const auto& opTeam = gameModel->getOppTeam();

        currentRobotObstacles.clear();
        currentRobotObstacles.reserve(myTeam.size() + opTeam.size() + 1);

        for(Robot* rob : myTeam)
            currentRobotObstacles.push_back(rob->getPosition());
        for(Robot* rob : opTeam)
            currentRobotObstacles.push_back(rob->getPosition());
    }

    /*********************************************************/
    static void sanitizeDestination(Point& dest, bool def_area_on)
    {
        // Clamp points that are outside the field
        dest.x = Measurements::clamp(dest.x, -HALF_FIELD_LENGTH+100.f,  HALF_FIELD_LENGTH-100.f);
        dest.y = Measurements::clamp(dest.y, -HALF_FIELD_WIDTH +100.f,  HALF_FIELD_WIDTH -100.f);

        DefenceArea da0(OUR_TEAM);
        DefenceArea da1(!OUR_TEAM);

        // Push points to edge of defence areas
        if(def_area_on)
        {
            da0.expelPoint(dest);
            da1.expelPoint(dest);
        }
    }
} //namespace impl

    void update()
    {
        if(impl::framesUntilUpdate-- == 0) {
            impl::genRobotObstacles();
            impl::framesUntilUpdate = FRAME_UPDATE_COUNT;
        }
    }

    Path genPath(const Point& start, Point end, bool avoidBall, bool use_def_areas)
    {
        // If the end point is not reachable, make it reachable
        //impl::sanitizeDestination(start, end, avoidBall, use_def_areas);

        Path path;
        path.push_back(start);
        path.push_back(end);
        impl::recursiveFPPA(&path, start, end, 1, avoidBall, use_def_areas);

        // If intermediate points in the path are not reachable, make them reachable
        for(auto it = path.begin()+1; it < path.end(); ++it)
            impl::sanitizeDestination(*it, use_def_areas);

        return path;
    }


    bool isObstacleInLine(const Point& start, const Point& end, Point* obsPosOut, bool avoidBall)
    {
        auto obstacle_info = impl::isObstacleinLine(start, end, avoidBall, false);

        if(obstacle_info.first) {
            if(obsPosOut != nullptr)
                *obsPosOut = obstacle_info.second;
            return Measurements::isClose(start, obstacle_info.second, 2500);
        } else {
            return false;
        }
    }

    const std::vector<Point>& getCurrentObstacles()
    {
        return impl::currentRobotObstacles;
    }

} //namespace FPPA
} //namespace Movement
