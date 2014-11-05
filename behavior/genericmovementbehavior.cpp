#include "behavior/genericmovementbehavior.h"

GenericMovementBehavior::GenericMovementBehavior()
    : movement(nullptr)
    , velocityMultipier(1.0)
    , useObstacleAvoid(true)
	{}

GenericMovementBehavior::GenericMovementBehavior(const ParameterList& list)
    : movement(nullptr)
    , velocityMultipier(1.0)    //Elusive bug solved
    , useObstacleAvoid(true)
{
    Point targetPoint;
    float targetAngle;
    bool  useObstacleAvoid;
    bool  gotTargetPoint = true, gotTargetAngle = true, gotObsAvoid = true;

    /* This should avoid being used where possible. */
    
    try {
        targetPoint = list.getParam<Point>("targetPoint");
    } catch(...) {
        gotTargetPoint = false;
    }
    try {
        targetAngle = list.getParam<float>("targetAngle");
    } catch(...) {
        gotTargetAngle = false;
    }
    try {
        useObstacleAvoid = list.getParam<bool>("obstacleAvoidance");
    } catch(...) {
        gotObsAvoid = false;
    }
    
    if(gotTargetPoint && gotTargetAngle && gotObsAvoid) {
        movement = new Movement::GoToPosition(targetPoint, targetAngle, useObstacleAvoid);
    } else if(gotTargetPoint && gotTargetAngle) {
        movement = new Movement::GoToPosition(targetPoint, targetAngle);
    } else if(gotTargetPoint) {
        movement = new Movement::GoToPosition(targetPoint);
    }
}


GenericMovementBehavior::~GenericMovementBehavior()
{
    delete movement;
	movement = nullptr;
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


