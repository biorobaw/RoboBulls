#include "penaltybehavior.h"

#if SIMULATED
    #define DIST 250
    #define ANGLE (7*M_PI/180)
    #define CLOSE_ENOUGH 110
#else
    #define DIST 350
    #define ANGLE (15*M_PI/180)
    #define CLOSE_ENOUGH 200
#endif

PenaltyBehavior::PenaltyBehavior(const ParameterList& list)
	: GenericMovementBehavior(list)
{
    UNUSED_PARAM(list);
    pb = initial;
}

/**
 * @brief PenaltyBehavior::perform Behavior that goes to the penalty point and kicks the ball once.
 * @param myRobot : The robot to perform the behavior on (usually myTeam[0])
 */
void PenaltyBehavior::perform(Robot * myRobot)
{
    GameModel *model = GameModel::getModel();
    Point robotPos = myRobot->getRobotPosition();
    Point ballPos = model->getBallPoint();
    float robotOrient = myRobot->getOrientation();
    float targetBallAngle = Measurments::angleBetween(model->getOpponentGoal(), ballPos);
    float ballTargetAngle = Measurments::angleBetween(ballPos, model->getOpponentGoal());
    Point behindBall = Point(DIST*cos(targetBallAngle)+ballPos.x, DIST*sin(targetBallAngle)+ballPos.y);

    move.setMovementTolerances(CLOSE_ENOUGH, ANGLE);
    move.setVelocityMultiplier(1);

    switch(pb)
    {
    case initial:
//        cout << "initial" << endl;
        pb = moving;
        target = behindBall;
        break;
    case moving:
//        cout << "moving" << endl;
        move.recreate(behindBall, ballTargetAngle, true);
        move.perform(myRobot, Movement::Type::Default);
        if (Measurments::isClose(robotPos,behindBall,CLOSE_ENOUGH) &&
            abs(Measurments::angleDiff(robotOrient, ballTargetAngle)) < ANGLE)
        {
            pb = approching;
            target = ballPos;
        }
        else if (Measurments::distance(target, behindBall) > CLOSE_ENOUGH)
        {
            pb = initial;
        }
        break;
    case approching:
//        cout << "approaching" << endl;
        move.recreate(ballPos, ballTargetAngle, true);
        move.perform(myRobot, Movement::Type::Default);
        if (Measurments::isClose(robotPos,ballPos,CLOSE_ENOUGH) &&
            abs(Measurments::angleDiff(robotOrient, ballTargetAngle)) < ANGLE )
        {
            pb = kicking;
        }
        else if (Measurments::distance(target, ballPos) > CLOSE_ENOUGH)
        {
            pb = initial;
        }
        break;

    case kicking:
//        cout << "kicking" << endl;
        {
            Skill::Kick kick;
            kick.perform(myRobot);
        #if PENALTY_BEHAVIOR_DEBUG
            cout<<"kicking performed!"<<endl;
        #endif
            pb = idling;
        }
        break;
    case idling:
//        cout << "idling" << endl;
        {
            Skill::Stop stop;
            stop.perform(myRobot);
        #if PENALTY_BEHAVIOR_DEBUG
            cout<<"idling performed!"<<endl;
        #endif
//            pb = initial;
        }
        break;
    }
}
