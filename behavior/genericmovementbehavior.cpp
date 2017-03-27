#include "behavior/genericmovementbehavior.h"

GenericMovementBehavior::GenericMovementBehavior
    (Point target, float angTarget, bool obsAvoid, bool ballAvoid)
    : velocityMultipier(1.0),
      useObstacleAvoid(true),
      useAvoidBall(false),
      movementFinished(false)
{
    setMovementTargets(target, angTarget, obsAvoid, ballAvoid);
}

//Override Behavior perform
void GenericMovementBehavior::perform(Robot* robot)
{
    perform(robot,Move::MoveType::Default);
}

/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/

//Specialized perform function
void GenericMovementBehavior::perform(Robot* robot, Move::MoveType type)
{
    movementFinished = movement.perform(robot, type);
}

bool GenericMovementBehavior::isFinished()
{
    return movementFinished;
}

void GenericMovementBehavior::setMovementTargets(Point targetPoint, float targetAngle)
{
    setMovementTargets(targetPoint, targetAngle, this->useObstacleAvoid, this->useAvoidBall);
}

void GenericMovementBehavior::setMovementTargets
    (Point targetPoint, float targetAngle, bool withObstacleAvoid, bool useAvoidBall)
{
   movement.updateGoal(targetPoint, targetAngle, withObstacleAvoid, useAvoidBall);

    /* Don't forget to re-set velocity multiplier, because this is lost with recreate.
     * It may be better to move this to the recreate function */
   movement.setVelocityMultiplier(this->velocityMultipier);
}

void GenericMovementBehavior::setVelocityMultiplier(float newMultiplier)
{
    this->velocityMultipier = newMultiplier;
    movement.setVelocityMultiplier(newMultiplier);
}

void GenericMovementBehavior::setMovementTolerances(float newDistTolerance, float newRotTolerance)
{
    movement.setMovementTolerances(newDistTolerance, newRotTolerance);
}
