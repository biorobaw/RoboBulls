#include "driveball.h"
#include "communication/robcomm.h"
#include "model/gamemodel.h"
#include "skill/gotopositionwithorientation.h"
#include "skill/basic_movement.h"
#include "include/globals.h"
#include "skill/skill.h"
#include "utilities/skillsequence.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "math.h"

#if SIMULATED
    #define CLOSE_ENOUGH 150
    #define CLOSE_ENOUGH 110
    #define ANGLE 7*M_PI/180
    #define DIST 50
#else
    #define CLOSE_ENOUGH 350
    #define ANGLE 5*M_PI/180
    #define DIST 650
    #define NXT_TO_BALL   200
#endif

namespace Skill
{
    DriveBall::DriveBall(Point targetPoint, double finalDirection)
    {
        targetPosition = targetPoint;
        direction = finalDirection;
        state = initial;
    }

    bool DriveBall::perform(Robot* robot)
    {
//        #if TRACE
//            cout << "Performing Skill::DriveBall" << endl;
//        #endif

        GameModel *gm = GameModel::getModel();

        behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                               DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
        closeToBall = new Point(NXT_TO_BALL*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                               NXT_TO_BALL*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
        float angleBallTarget = Measurments::angleBetween(gm->getBallPoint(), targetPosition);

        switch(state)
        {
        case initial:
            cout<<"Drive ball initial state"<<endl;
            state = moveBehindBall;
            goal = behindBall;
            skill = new GoToPositionWithOrientation (*behindBall, angleBallTarget);
            break;
        case moveBehindBall:
            cout<< "move behind the ball" << endl;
            if (Measurments::distance(*goal, *behindBall) > CLOSE_ENOUGH)
            {
                state = moveBehindBall;
                skill = new GoToPositionWithOrientation (*behindBall, angleBallTarget);
                goal = behindBall;
            }
            else if (Measurments::isClose(robot->getRobotPosition(), *behindBall, CLOSE_ENOUGH)
                && abs(Measurments::angleDiff(robot->getOrientation(), angleBallTarget)) < ANGLE)
            {
                state = moveTowardBall;
//                skill = new GoToPositionWithOrientation (*closeToBall, angleBallTarget);
                skill = new GoToPositionWithOrientation (gm->getBallPoint(), angleBallTarget);
                *goal = gm->getBallPoint();
            }

            break;
        case moveTowardBall:
            cout <<"Move toward the ball"<<endl;

            if(Measurments::isClose(robot->getRobotPosition(), *closeToBall, 150)
               && abs(Measurments::angleDiff(robot->getOrientation(), angleBallTarget)) < ANGLE)
            {
                state = driveBall;
                GoToPositionWithOrientation *gotoPos = new GoToPositionWithOrientation (targetPosition, direction);
                //reducing speed when robot has the ball in order to keep the ball
                gotoPos->setVelocityMultiplier(.4);
                skill = gotoPos;
            }
            else if(Measurments::distance(*goal, gm->getBallPoint()) > CLOSE_ENOUGH)
            // If the ball has changed position
            {
                state = moveBehindBall;
                goal = behindBall;
                skill = new GoToPositionWithOrientation (*behindBall, angleBallTarget);
            }
            break;
        case driveBall:
            cout <<"drive the ball"<<endl;
            // Orientation
            if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 700))
            {
                state = moveBehindBall;
                goal = behindBall;
                skill = new GoToPositionWithOrientation (*behindBall, angleBallTarget);
            }
            break;
        }

        skill->perform(robot);

        return false;
    }
}
