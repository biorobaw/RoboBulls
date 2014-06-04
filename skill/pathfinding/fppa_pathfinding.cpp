#include <math.h>
#include <algorithm>
#include <utility>
#include <iostream>
#include "model/gamemodel.h"
#include "skill/pathfinding/fppa_pathfinding.hpp"
#include "include/globals.h"

/* Implementation of the Fast Path Planning Algorithm
 * JamesW
 */
namespace FPPA
{
namespace impl
{
	float perpendicularDistance(const Point p0, const Point LStart, const Point LEnd)
	{
		float y2 = LEnd.y;
		float y1 = LStart.y;
		float x2 = LEnd.x;
		float x1 = LStart.x;

        return fabs( (y2-y1)*p0.x - (x2-x1)*p0.y - x1*y2 + x2*y1 )
				/ Measurments::distance(LStart, LEnd);
	}

	bool insideRadiusRectangle(Point p0, Point p1, Point p2)
	{
	    float rectTop = std::min(p1.y, p2.y) - ROBOT_SIZE;
	    float rectBottom = std::max(p1.y, p2.y) + ROBOT_SIZE;
	    float rectLeft = std::min(p1.x, p2.x) - ROBOT_SIZE;
	    float rectRight = std::max(p1.x, p2.x) + ROBOT_SIZE;

	    return (p0.x > rectLeft && p0.x < rectRight) && (p0.y > rectTop && p0.y < rectBottom);
	}

	std::pair<bool, Point> isObstacleinLine(std::vector<Point>* obstacles, Point& beginPos, Point& endPos)
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
		printf("[FPPA] Checking Line (%f,%f)-(%f,%f)\n", beginPos.x, beginPos.y, endPos.x, endPos.y);
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


	Point findSearchDirection(Point& initialPos, Point& endPos, int sign)
	{
		float theta = Measurments::angleBetween(initialPos, endPos);

		/* Reducing the search size by a factor can help eliminate
		   the jagged edges in the path, but risks cutting corners
		   too close around obstacles.
		   */
        float dx = ROBOT_SIZE * cos(theta + M_PI_2);
        float dy = ROBOT_SIZE * sin(theta + M_PI_2);

		return Point(sign * dx, sign * dy);
	}


	void buildPathimpl(Path* results, std::vector<Point>* obstacles,
					   Point beginPos, Point endPos, int sign, int depth)
	{
		if(depth >= MAX_RECURSION_DEPTH)
			return;

		auto obstacle_info = isObstacleinLine(obstacles, beginPos, endPos);

		if(obstacle_info.first == true) { 		//Obstacle was found
			Point deltaPoint = findSearchDirection(beginPos, endPos, sign);
			Point subGoal;
			int i = 1;

			while(obstacle_info.first && (i < 10)) {
				subGoal = obstacle_info.second + Point(i * deltaPoint.x,  i * deltaPoint.y);
				obstacle_info.first = isObstacleAtPoint(obstacles, subGoal);
				i += 1;
			}

			/* Find the ending point and insert the new point before it
				(Possibly update this knowing the pos of the point)
				*/
			auto it = std::find(results->begin(), results->end(), endPos);
			results->insert(it, subGoal);

			/* Recursively test the paths from the beginning to the subGoal and from
			   the subGoal to the end
			   */
			buildPathimpl(results, obstacles, beginPos, subGoal, sign, depth + 1);
			buildPathimpl(results, obstacles,  subGoal,  endPos, sign, depth + 1);
		}
	}


	std::pair<Path, Path> findBothPaths(std::vector<Point>* obstacles, Point& start, Point& end)
	{
		Path topPath, bottomPath;

		topPath.push_back(start);
		topPath.push_back(end);
		bottomPath.push_back(start);
		bottomPath.push_back(end);
		
		impl::buildPathimpl(&topPath, obstacles, start, end, -1, 0);
		impl::buildPathimpl(&bottomPath, obstacles, start, end,  1, 0);

		return std::pair<Path, Path>(topPath, bottomPath);
	}

} //namespace impl

    Path findShortestPath(Point start, Point end)
    {
		GameModel* mod = GameModel::getModel();
		float totalDistTop = 0;
		float totalDistBottom = 0;
		std::vector<Point> obstacles;

        obstacles.reserve(mod->getMyTeam().size() + mod->getOponentTeam().size() + 1);

        for(Robot* rob : mod->getMyTeam())
            obstacles.push_back(rob->getRobotPosition());
        for(Robot* rob : mod->getOponentTeam())
			obstacles.push_back(rob->getRobotPosition());

        //obstacles.push_back(mod->getBallPoint());
		
		/* In the likely case the the begin point was a robot's position, remove
		   that point from the obstacles to avoid the robot detecting itself
		   as an obstacle. 
           */
        std::remove_if(obstacles.begin(), obstacles.end(),
            [&](Point& pt){return Measurments::isClose(pt, start, ROBOT_SIZE);});

        std::remove_if(obstacles.begin(), obstacles.end(),
            [&](Point& pt){return Measurments::isClose(pt, end, ROBOT_SIZE);});

		
		/* Sort the range on how close each point is to the start.
		   May help with forming proper paths by always finding the 
		   closest point first.
		   */
		std::sort(obstacles.begin(), obstacles.end(),
			[&](Point a, Point b) {
				return Measurments::distance(start, a) < Measurments::distance(start, b);
			});

	#if FPPA_DEBUG
        std::cout << "[FPPA] All Obstacles: " << std::endl;
        for(Point pt : obstacles) std::cout << pt.toString() << std::endl;
	#endif

	
		std::pair<Path, Path> foundPaths = impl::findBothPaths(&obstacles, start, end);


		for(auto it = foundPaths.first.begin(); it != foundPaths.first.end()-1; ++it)
			totalDistTop += Measurments::distance(*it, *(it+1));

		for(auto it = foundPaths.second.begin(); it != foundPaths.second.end()-1; ++it)
			totalDistBottom += Measurments::distance(*it, *(it+1));

	#if FPPA_DEBUG
        printf("[FPPA] TopD:%0.2f; BotD:%0.2f\n", totalDistTop, totalDistBottom);
		if(totalDistTop <= totalDistBottom) {
			puts("[FPPA] The better path was the top one");
		} else {
			puts("[FPPA] The better path was the bottom one");
		}
	#endif

		if(totalDistTop <= totalDistBottom) {
			return foundPaths.first;
		} else {
			return foundPaths.second;
		}
    }
}










