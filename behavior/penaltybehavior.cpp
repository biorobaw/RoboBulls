#include "penaltybehavior.h"

#if SIMULATED
    #define DIST 250
    #define ANGLE (7*M_PI/180)
    #define CLOSE_ENOUGH 110
#else
    #define DIST 270
    #define ANGLE (7*M_PI/180)
    #define CLOSE_ENOUGH 170
#endif

PenaltyBehavior::PenaltyBehavior()
    : GenericMovementBehavior()
{
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
    bool ballInFront = abs(Measurments::angleDiff(robotOrient, Measurments::angleBetween(robotPos, ballPos))) < ANGLE*3;

    switch(pb)
    {
    case initial:
        cout << "initial" << endl;
        pb = moving;
        target = behindBall;
        break;
    case moving:
        cout << "moving" << endl;
        setMovementTargets(behindBall, ballTargetAngle, true);
        GenericMovementBehavior::perform(myRobot, Movement::Type::Default);
//        cout << "1\t" << Measurments::distance(robotPos,behindBall) << endl;
//        cout << "2\t" << abs(Measurments::angleDiff(robotOrient, ballTargetAngle))/M_PI*180 << endl;
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
        cout << "approaching" << endl;
//        cout << "1\t" << Measurments::distance(robotPos,ballPos) << endl;
//        cout << "2\t" << abs(Measurments::angleDiff(robotOrient, ballTargetAngle))/M_PI*180 << endl;
//        cout << "3\t" << abs(Measurments::angleDiff(robotOrient, Measurments::angleBetween(robotPos, ballPos)))/M_PI*180 << endl;
        setMovementTargets(ballPos, ballTargetAngle, false);
        GenericMovementBehavior::perform(myRobot, Movement::Type::Default);
        if (Measurments::isClose(robotPos,ballPos,CLOSE_ENOUGH) &&
            abs(Measurments::angleDiff(robotOrient, ballTargetAngle)) < ANGLE &&
                ballInFront)
        {
            pb = kicking;
        }
        else if (Measurments::distance(target, ballPos) > CLOSE_ENOUGH)
        {
            pb = initial;
        }
        break;

    case kicking:
        cout << "kicking" << endl;
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
        cout << "idling" << endl;
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
