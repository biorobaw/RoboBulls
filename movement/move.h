#ifndef MOVEMENT_MOVE_H
#define MOVEMENT_MOVE_H

#include <deque>
#include "include/config/globals.h"
#include "movement/pathfinding/fppa_pathfinding.h"
#include "movement/movetype.h"

class Robot;

namespace Movement
{

/* Separate from a skill, Movement::Move represents
 * a layer lower than those in Skill. Movement contains
 * routines for robot movement, including pathfinding,
 * GotoPosition, three/four wheel omni movement, and the differential
 * control modules "closed loop control (CLC)."
 *
 * This is the Movement's core base class, it has some standardized features now:
 * - Velocity multiplier via setVelocityMultiplier
 *   1.0 is normal velocity, 0.0 is no velocity. This is different from before.
 * - Obstacle avoidance or standard movement via withObstacleAvoid bool;
 *   However, this is abstracted from the velocity calculators; it only
 *   asks the calculators to compute velocity to the next points.
 */
class Move
{
public:
    Move();
    Move(Point targetPoint, float targetAngle = UNUSED_ANGLE_VALUE,
         bool withObstacleAvoid = true, bool avoidBall = true);
    virtual ~Move();
    
    /* "Recreates" the Movement object; repositioning the target point to a new
     * point, and the target angle to a new angle. Also has to option to toggle
     * obstacle avoidance or not
     */
    void recreate(Point targetPoint,
                  float targetAngle = UNUSED_ANGLE_VALUE,
                  bool withObstacleAvoid = true, bool avoidBall = true);
    
    /* A scalar applied to the calculated velocity when set to the robot. 
     * 1.0 is default, 0.0 means no velocity. Setting this over 2 isn't very good
     */
    void setVelocityMultiplier(float newMultiplier);
    
    /* This function is used to set the "recreation" tolerances. This object will not
     * recompute pathfinding and angle adjustments if the difference between current
     * and new parameters passed to `recreate` are less than these values
     */
    void setRecreateTolerances(float distTolerance, float angleTolerance);

    /* Sets the distance and angle tolerance that are compared against when
     * determining if a robot is close to the target or not
     */
    void setMovementTolerances(float distTolerance, float angleTolerance);
    
    /* Perform movement on the robot */
    bool perform(Robot* robot, Type moveType = Type::Default);

protected:
    float lfront, lback, rfront, rback;  //rob->type() == fourWheelOmni
    float left, right;                   //rob->type() == differential;
    float back;                          //rob->type() == threeWheelOmni;

    /* Override this function in a derived class to provide the calculated
     * velocities to a general point targetPoint, and general angle targetAngle.
     * Store the results in *this (lfront, lback... etc); but in this function
     * do not set the velocities on the robot.
     */
    virtual void calculateVels
        (Robot* rob, Point targetPoint, float targetAngle, Type moveType) = 0;
        
private:
    Point m_targetPoint;        //The requested final target point
    float m_targetAngle;        //The requested final target angle
    float velMultiplier;        //Velocity multipler added to calculated vels

    //States
    bool  isInitialized;        //If recreate() has been called once (or ctor)
    bool  useObstacleAvoid;     //Do we use obstacle avoidance?
    bool  useAvoidBall;         //Do we avoid the ball?
    bool  hasFoundPathEnd;      //Have we found the end of the path?
    bool  currentPathIsClear;   //Is the current path clear?

    //FPPA info
    float nextTargetAngle;
    float nextDistTolerance;
    Point nextPoint;
    FPPA::PathInfo      pathInfo;
    std::deque<Point>   pathQueue;
    FPPA::PathDirection lastDirection;
    std::vector<Point>  lastObstacles;
    long  lastLineDrawnTime;
    
    //Default and user-set recreation (see recreate()) tolerances
    float recrDistTolerance  = 30;
    float recrAngleTolerance = 3*M_PI/180;
    float lastDistTolerance  = 50; //CLC guarantees this
    float lastAngTolerance   = 5*M_PI/180;

    //Obstacle avoidance functions
    Point updatePathQueue(Robot *robot);
    bool  determinePathClear(Robot *robot) const;
    bool  calcObstacleAvoidance(Robot* rob, Type moveType);
    void  assignNewPath(const Point& robotPoint);
    void  getCollisionState(Robot* robot, bool& collided, bool& yielding) const;

    //Regular movement
    bool calcRegularMovement(Robot* rob, Type moveType);

    //Utility to switch over Robot::Type and set actual velocities
    void setVels(Robot* robot);
};

}

/*
Movement::Move* myMove;
...
myMove = new Movement::GoToPosition(targetPoint, targetAngle);
...
myMove->setVelocityMultiplier(2);
myMove->perform(robot);
myMove->perform(robot, Movement::Differential::SharpTurns);
*/

#endif
