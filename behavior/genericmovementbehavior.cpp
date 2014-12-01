#include "behavior/genericmovementbehavior.h"


GenericMovementBehavior::GenericMovementBehavior
    (Point target, float angTarget, bool obsAvoid, bool ballAvoid)
    : movement(nullptr)
    , velocityMultipier(1.0)    //Elusive bug solved
    , useObstacleAvoid(true)
{  
    movement = new Movement::GoToPosition(target, angTarget, obsAvoid, ballAvoid);
}


GenericMovementBehavior::~GenericMovementBehavior()
{
    delete movement;
    movement = nullptr;
}

/* New: Post-26th:
 * Default perform function that goes to the target points.
 * Should have been here a long time ago
 */
void GenericMovementBehavior::perform(Robot* robot)
{
    perform(robot, Movement::Type::SharpTurns);
}


/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/


void GenericMovementBehavior::perform(Robot* robot, Movement::Type type)
{
    if(movement != nullptr)
        movement->perform(robot, type);
}


void GenericMovementBehavior::setMovementTargets(Point targetPoint, float targetAngle)
{
    setMovementTargets(targetPoint, targetAngle,
                       this->useObstacleAvoid, this->useAvoidBall);
}


void GenericMovementBehavior::setMovementTargets
    (Point targetPoint, float targetAngle, bool withObstacleAvoid, bool useAvoidBall)
{
    if(movement != nullptr) {
        movement->recreate(targetPoint, targetAngle, withObstacleAvoid, useAvoidBall);
    } else {
        /* Here, movement is a nullptr, so there is no need to delete it
         * before creating a new one
         */
        movement = new Movement::GoToPosition(targetPoint, targetAngle, withObstacleAvoid);
    }
    /* Don't forget to re-set velocity multiplier, because this is lost with recreate.
     * It may be better to move this to the recreate function
     */
    movement->setVelocityMultiplier(this->velocityMultipier);
}


void GenericMovementBehavior::setVelocityMultiplier(float newMultiplier)
{
    this->velocityMultipier = newMultiplier;
    if(movement != nullptr)
        movement->setVelocityMultiplier(newMultiplier);
}

void GenericMovementBehavior::setMovementTolerances(float newDistTolerance, float newRotTolerance)
{
    this->newDistTolerance = newDistTolerance;
    this->newRotTolerance = newRotTolerance;
    if(movement != nullptr)
        movement->setMovementTolerances(newDistTolerance,newRotTolerance);
}


