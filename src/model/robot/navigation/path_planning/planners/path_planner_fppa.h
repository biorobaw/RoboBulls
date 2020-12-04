#ifndef FPPA_PATHFINDING_H
#define FPPA_PATHFINDING_H

#include "model/robot/navigation/path_planning/path_planner.h"
#include "vector"


/*! @brief Fast Path Planning Algorithm
 * @author JamesW, Muhaimen Shamsi
 * @details This class extends the abstract class PathPlanner implementing
 * the Fast Path Planning Algorithm (FPPA) as described in the [paper](http://fei.edu.br/rcs/2014/RegularPapers/robocupsymposium2014_submission_55.pdf)
 * "Fast Path Planning Algorithm for the RoboCub Small Size League" by Rodriguez S., Rojas E., Calderon J. et al.
 * The algorithm is meant to provide reliable path-finding in real time, such as in
 * the case of the RoboCup league. */

class PathPlannerFPPA : public PathPlanner
{
public:

    PathPlannerFPPA(Robot* self);
    ~PathPlannerFPPA();

    Path genPath(Point goal, bool avoidBall = true) override;


private:

    /*!
     * \brief recursiveFPPA implements the FPPA algorithm as described in [paper](http://fei.edu.br/rcs/2014/RegularPapers/robocupsymposium2014_submission_55.pdf)
     * \param path      current path to last sub goal
     * \param next_goal next sub goal to add
     * \param depth     recursion depth
     */
    void recursiveFPPA(Path& path, const Point& next_goal, int depth);

    /*!
     * \brief chooseSubGoal used by recursiveFPPA to choose a subgoal when the segment from the current subgoal to the next
     * intersects an obstacle
     */
    inline Point chooseSubGoal(const Point& start, const Point& end, const Point& obstacle);

    /*!
     * \brief segmentIntersectsObstacle used by recursiveFPPA to check whether the segment from the current subgoal to the next
     * intersects an obstacle.
     * \param start The segment starting point
     * \param end The segment ending point
     * \param obstacle_position Position of the obstacle found (used as a returned value)
     * \return
     */
    inline bool segmentIntersectsObstacle(const Point& start, const Point& end, Point& obstacle_position);

    /*! @brief Checks whether the specified point is within the obstacle space */
    inline bool isPointAtObstacle(const Point& point);



    /*!
     * \brief This function moves the given point to a valid close by location (if necessary).
     * \details Used to make sure the path
     * lies within the field and does not use the deffence areas unless the robot is the goalie.
     */
    inline void moveToValidLocation(Point& point);


    std::vector<Point> robotObstacles;
    Point goal;
    bool avoid_ball;
    bool is_goalie;

};



#endif // FPPA_PATHFINDING_H
