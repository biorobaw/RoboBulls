#ifndef GOTOPOSITON_H
#define GOTOPOSITON_H

#include "model/robot.h"
#include "movement/differential/differential_pilot.h"
#include "movement/four_wheel_omni/four_wheel_omni_pilot.h"
#include "movement/three_wheel_omni/three_wheel_omni_pilot.h"
#include "movement/pathfinding/fppa_pathfinding.h"
#include "movement/movetype.h"
#include "include/config/tolerances.h"
#include "include/config/globals.h"

#include <deque>
#include <math.h>

namespace Move{
/*! @file
 * Top-level interface to provide robot movement.
 * Handles obstacle avoidance, collision recovery, and multiple movement types.
 * This class is fit for use by Skills directly for generic movement on a robot.
 */

class GoToPose
{

public:
    GoToPose();
    virtual ~GoToPose();

    /*! @brief Initialize the GoToPose object
     * @details Provide an initial target, angle, and obstacle avoidance specifications
     * @see recreate */
    GoToPose(Point targetPoint, float targetAngle = UNUSED_ANGLE_VALUE,
         bool withObstacleAvoid = true, bool avoidBall = true);

    /*! @brief "Recreates" the GoToPose object
     * Recreating the movement object each iteration allows the Robot to
     * effectively move from Point A to any Point B. The purpose of this
     * function is to be called lazilly each iteration to move to a
     * possibly moving target. The object is not recreated unless the new
     * targetPoint is sufficiently away from the previous targetPoint.
     */
    void updateGoal(Point targetPoint,
                  float targetAngle = UNUSED_ANGLE_VALUE,
                  bool withObstacleAvoid = true,
                  bool avoidBall = true);

    /*! @brief Set a scalar applied to the calculated velocity when set to the robot.
     *  @details 1.0 is default, 0.0 means no velocity. Setting this over 2 isn't very good */
    void setVelocityMultiplier(float newMultiplier);

    /*! @brief Sets the creation distance and angle tolerances
     *
     * This function is used to set the "recreation" tolerances. This object will not
     * recompute pathfinding and angle adjustments if the difference between current
     * and new parameters passed to `recreate` are less than these values
     * @param distTolerance Required point target distance difference
     * @param angleTolerance Required angle target difference (radians)
     * @see recreate */
    void setRecreateTolerances(float distTolerance, float angleTolerance);

    /*! @brief Set final distance and angle movement tolerances
     *
     * This function set tolerances as to how close a robot needs to be to the
     * target to be considered at that point.*/
    void setMovementTolerances(float distTolerance, float angleTolerance);

    /*! @brief Performs the configured movement on the robot
     * @param robot The robot to be moved
     * @param Type the type of movement; see Type for information
     * @see movetype.h
     * @see Type */
    bool perform(Robot* robot, MoveType moveType = MoveType::Default);

private:
    Point final_target_point;    //The requested final target point
    float final_target_angle;    //The requested final target angle
    float vel_multiplier;    //Velocity multipler added to calculated vels

    // States
    bool is_initialized;       //If recreate() has been called once (or ctor)
    bool avoid_obstacles;      //Do we use obstacle avoidance?
    bool avoid_ball;           //Do we avoid the ball?

    // Obstacle Avoidance info
    float next_target_angle;
    float next_dist_tolerance;
    Point next_point, next_next_point;
    std::deque<Point>   path_queue;
    long  lastLineDrawnTime;

    // Default and user-set recreation (see recreate()) tolerances
    float recrDistTolerance  = 10;
    float recrAngleTolerance = 1*M_PI/180;
    float last_dist_tolerance  = DIST_TOLERANCE;
    float lastAngTolerance   = ROT_TOLERANCE;

    // Obstacle avoidance functions
    bool performObstacleAvoidance(Robot* rob, MoveType moveType);
    bool performNonAvoidMovement(Robot* rob, MoveType moveType);
    void assignNewPath(const Point& robotPoint, bool is_goalie);
    void getCollisionState(Robot* robot, bool& collided, bool& yielding) const;

    // General Motion functions
    void calcAndSetVels(Robot* rob, Point targetPoint, float targetAngle,
                        Point next_point, MoveType moveType);

    // Pilots
    DifferentialPilot diff_p;
    ThreeWheelOmniPilot three_wheel_omni_p;
    FourWheelOmniPilot four_wheel_omni_p;
};

}
#endif
