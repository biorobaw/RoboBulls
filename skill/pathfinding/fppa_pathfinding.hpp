#ifndef FPPA_PATHFINDING_H
#define FPPA_PATHFINDING_H

/*  Fast Path Planning Algorithm
	Contributor: James Waugh

	This namespace implements the Fast Path Planning Algorithm (FPPA)
	as described in the paper "Fast Path Planning Algorithm for the RoboCub Small Size League"
	by Rodriguez S., Rojas E., Calderon J. et al.

	The algorithm is meant to provide reliable path-finding in real time, such as in
	the case of the RoboCup league.
	*/
#include <vector>
#include "utilities/point.h"
#include "utilities/measurments.h"

/*  Robot Radius and Ball Radius should be standardized
	in some other file. We need a constants.h
	*/
#define MAX_RECURSION_DEPTH  3
#define ROBOT_RADIUS 		 200

#define FPPA_DEBUG 1

namespace FPPA
{
	typedef std::vector<Point> Path;
	
    Path findShortestPath(Point start, Point end);
}

#endif // FPPA_PATHFINDING_H
