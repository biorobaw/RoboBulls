#include <math.h>
#include <algorithm>
#include <iostream>
#include "include/config/tolerances.h"
#include "utilities/measurments.h"
#include "utilities/comparisons.h"
#include "utilities/region.h"
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
#define MAX_RECURSION_DEPTH 5
/* Defines the number of calls to pathfindingBegin (to update FPPA) must be made before
 * the state of all obstacles is updated. This is for efficiency reaons
 */
#define FRAME_UPDATE_COUNT 10
/* Defines the minimum distance an obstacle must be from a line segment in the path
 * before it is considered an obstacle. Using multiples of ROBOT_SIZE is ideal.
 */
#define OBSTACLE_LINE_DIST  (2 * ROBOT_SIZE)
/* Defines, when sub-divding a path to avoid an obstalce, the perpendicular distance
 * it travels from the segment to avoid it. Reducing the search size by a factor can
 * help eliminate the jagged edges in the path, but risks cutting corners too close around
 * obstacles. Multiples of ROBOT_RADIUS are ideal
 */
#define SEARCH_DIRECTION_DIST (2*ROBOT_RADIUS)

/************************************************************************/

namespace FPPA
{
namespace impl
{
    const Point fieldTopLeft = Point(-FIELD_LENGTH, -FIELD_WIDTH);
    const Point fieldBotRight = Point(FIELD_LENGTH, FIELD_WIDTH);
    int framesUntilUpdate = 0;
    std::vector<Point> currentFrameObstacles;

    static bool insideRadiusRectangle(const Point& p0, const Point& p1, const Point& p2)
    {
        float rectTop    = std::min(p1.y, p2.y) - ROBOT_RADIUS;
        float rectBottom = std::max(p1.y, p2.y) + ROBOT_RADIUS;
        float rectLeft   = std::min(p1.x, p2.x) - ROBOT_RADIUS;
        float rectRight  = std::max(p1.x, p2.x) + ROBOT_RADIUS;

        return (p0.x > rectLeft && p0.x < rectRight)
            && (p0.y > rectTop  && p0.y < rectBottom);
    }

    /*********************************************************/

    static std::pair<bool, Point>
    isObstacleinLine(const Point& beginPos, const Point& endPos, bool avoidBall)
    {
        bool  obstacle_found = false;
        Point obstacle_position;

        for(const Point& obstacle : currentFrameObstacles)
        {
            /* Here we exclude obstacle points that are too close to the starting point,
             * or exclude the ball if avoidBall is not on. */
            if(Measurments::isClose(obstacle, beginPos, ROBOT_RADIUS))
                continue;
            if(Measurments::isClose(gameModel->getBallPoint(), obstacle, 80) and not(avoidBall))
                continue;

            obstacle_found = Measurments::lineDistance(obstacle, beginPos, endPos) < OBSTACLE_LINE_DIST &&
                insideRadiusRectangle(obstacle, beginPos, endPos);

            if(obstacle_found) {
                obstacle_position = obstacle;
                break;
            }
        }

    #if FPPA_DEBUG
        printf("[FPPA] Checking Line (%f,%f)-(%f,%f)\n", beginPos.x, beginPos.y, endPos.x, endPos.y);
        if(obstacle_found)
            printf("[FPPA] Found: %.0f,%.0f\n", obstacle_position.x, obstacle_position.y);
    #endif

        return std::pair<bool, Point>(obstacle_found, obstacle_position);
    }


    static bool isObstacleAtPoint(const Point& toCheck)
    {
        if(Region::goalLeftRegion.contains(toCheck) || Region::goalRightRegion.contains(toCheck))
            return true;
        return Comparisons::isDistanceToLess(toCheck, ROBOT_SIZE).any_of(currentFrameObstacles);
    }

    /*********************************************************/

    static Point findSearchDirection(const Point& initialPos, const Point& endPos, int sign)
    {
        /* The search point is `SEARCH_DIRECTION_DIST` points perpendicularly from
         * the line between initialPos and endPos, with sign determining the direction */
        float theta = Measurments::angleBetween(initialPos, endPos);
        float dx = SEARCH_DIRECTION_DIST * cos(theta + M_PI_2);
        float dy = SEARCH_DIRECTION_DIST * sin(theta + M_PI_2);
        return Point(sign * dx, sign * dy);
    }

    /*********************************************************/

    static void buildPathimpl(Path* results, const Point& beginPos,
                              const Point& endPos,int sign, int depth, bool avoidBall)
    {
        if(depth >= MAX_RECURSION_DEPTH)
            return;

        auto obstacle_info = isObstacleinLine(beginPos, endPos, avoidBall);

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
            buildPathimpl(results, beginPos, subGoal, sign, depth + 1, avoidBall);
            buildPathimpl(results, subGoal,  endPos, sign, depth + 1, avoidBall);
        }
    }

    /*********************************************************/

    /* This is the function that communicates outside the FPPA namespace to define
     * obstacle information. In our case, that is getting the GameModel and all the robots
     * from that. However, I would like to have this more generalized in the future.
     */
    static void buildObstacleCollection()
    {
        const auto& myTeam = gameModel->getMyTeam();
        const auto& opTeam = gameModel->getOponentTeam();

        currentFrameObstacles.clear();
        currentFrameObstacles.reserve(myTeam.size() + opTeam.size() + 1);

        for(Robot* rob : myTeam)
            currentFrameObstacles.push_back(rob->getRobotPosition());
        for(Robot* rob : opTeam)
            currentFrameObstacles.push_back(rob->getRobotPosition());

    #if FPPA_DEBUG
           std::cout << "[FPPA] All Obstacles: " << std::endl;
           for(Point pt : *obstacles) std::cout << pt.toString() << std::endl;
    #endif
    }

    /*********************************************************/

    static std::pair<Path, Path>
    findBothPaths(const Point& start, const Point& end, bool avoidBall)
    {
        Path topPath, bottomPath;

        topPath.push_back(start);
        topPath.push_back(end);
        bottomPath.push_back(start);
        bottomPath.push_back(end);

        impl::buildPathimpl(&topPath, start, end, -1, 0, avoidBall);
        impl::buildPathimpl(&bottomPath, start, end,  1, 0, avoidBall);

        return std::move(std::make_pair(std::move(topPath), std::move(bottomPath)));
    }

    /*********************************************************/

    static void sanitizePoint(Point& pt)
    {
        //Used to clamp point to inside field
        pt.x = Measurments::clamp(pt.x,-FIELD_LENGTH+100.f, FIELD_LENGTH-100.f);
        pt.y = Measurments::clamp(pt.y, -FIELD_WIDTH+100.f,  FIELD_WIDTH-100.f);
    }

    static bool isValidPath(const Path& p)
    {
        //Returns true if no point in the path is within 20% of the edge of the field
        //Exclude the first point becasue it is the robot
        return std::none_of(p.begin()+1, p.end(),[](const Point& t) {
            return abs(t.x) >= 0.80*FIELD_LENGTH || abs(t.y) >= 0.80*FIELD_WIDTH;});
    }

    static float getPathLength(const Path& p)
    {
        //Generic loop summing the distance between nodes in the path
        float totalDist = 0;
        for(auto it = p.begin(); it != p.end()-1; ++it)
            totalDist += Measurments::distance(*it, *(it+1));
        return totalDist;
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


    PathInfo findShortestPath(const Point& start, const Point& end, bool avoidBall,
                              PathDirection pathHint, float unlessValue)
    {
        if(avoidBall) impl::currentFrameObstacles.push_back(gameModel->getBallPoint());

        std::pair<Path,Path> foundPaths = impl::findBothPaths(start, end, avoidBall);

        if(avoidBall) impl::currentFrameObstacles.pop_back();

        //Create PathInfos: pair  of {points vector, direction top/bottom}
        PathInfo topPathInfo = std::make_pair(std::move(foundPaths.first),  PathDirection::Top);
        PathInfo botPathInfo = std::make_pair(std::move(foundPaths.second), PathDirection::Bottom);

        //Invalidate any path going along edge of field, and
        //Limit all points in both paths paths to being inside the field
        for(Point& p : topPathInfo.first) impl::sanitizePoint(p);
        for(Point& p : botPathInfo.first) impl::sanitizePoint(p);
        if(!impl::isValidPath(topPathInfo.first)) return botPathInfo;
        if(!impl::isValidPath(botPathInfo.first)) return topPathInfo;

        //Get distance and valid status of each
        float topTotalDist   = impl::getPathLength(topPathInfo.first);
        float botTotalDist   = impl::getPathLength(botPathInfo.first);

        //User requested top or bottom path explicitly
        if(pathHint != PathDirection::None) {
            PathInfo* chosenPathInfo = &topPathInfo;
            PathInfo* otherPathInfo  = &botPathInfo;
            float selectedDist = topTotalDist;
            float otherDist = botTotalDist;

            if(pathHint == PathDirection::Bottom) {
                chosenPathInfo = &botPathInfo;
                otherPathInfo  = &topPathInfo;
                selectedDist = botTotalDist;
                otherDist = topTotalDist;
            }

            if(unlessValue != -1.0) {
                //The other path is at least "unlessValue" percent shorter
                if(selectedDist - (selectedDist * unlessValue) > otherDist)
                    return *otherPathInfo;
            }
            return *chosenPathInfo;
         }

        //Otherwise, selects PathInfo top or bottom strictly based on best distance.
        PathInfo& chosenPathInfo = topPathInfo;
        if(botTotalDist < topTotalDist)
            chosenPathInfo = botPathInfo;
        return chosenPathInfo;
    }


    bool isObstacleInLine(const Point& start, const Point& end, Point* obsPosOut, bool avoidBall)
    {
        if(avoidBall)
            impl::currentFrameObstacles.push_back(gameModel->getBallPoint());

        auto obstacle_info = impl::isObstacleinLine(start, end, avoidBall);

        if(avoidBall)
            impl::currentFrameObstacles.pop_back();

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

} //namespace FPPA
