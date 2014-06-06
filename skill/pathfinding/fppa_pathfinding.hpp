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
	typedef std::vector<Point> Path;
	
    Path findShortestPath(Point start, Point end);
}

#endif // FPPA_PATHFINDING_H
