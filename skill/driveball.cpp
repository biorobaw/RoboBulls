#include "communication/robcomm.h"
#include "model/gamemodel.h"
#include "movement/gotopositionwithorientation.h"
#include "skill/stop.h"
#include "skill/skill.h"
#include "math.h"
#include "driveball.h"

#if SIMULATED
    #define CLOSE_ENOUGH 150
    #define ANGLE 7*M_PI/180
    #define DIST 50
    #define NXT_TO_BALL 100
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
        skill = nullptr;
    }

    DriveBall::~DriveBall()
    {
        delete skill;
    }

    bool DriveBall::perform(Robot* robot)
    {
//        #if TRACE
//            cout << "Performing Skill::DriveBall" << endl;
//        #endif

        GameModel *gm = GameModel::getModel();

        behindBall = Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                               DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
        closeToBall = Point(NXT_TO_BALL*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                               NXT_TO_BALL*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
        float angleBallTarget = Measurments::angleBetween(gm->getBallPoint(), targetPosition);

        switch(state)
        {
        case initial:
            cout<<"Drive ball initial state"<<endl;
            state = moveBehindBall;
            goal = behindBall;
            delete skill;
            skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget, false);
            break;
        case moveBehindBall:
            cout<< "move behind the ball" << endl;
            if (Measurments::distance(goal, behindBall) > CLOSE_ENOUGH)
            {
                state = moveBehindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget, false);
                goal = behindBall;
            }
            else if (Measurments::isClose(robot->getRobotPosition(), behindBall, CLOSE_ENOUGH)
                && abs(Measurments::angleDiff(robot->getOrientation(), angleBallTarget)) < ANGLE)
            {
                state = moveTowardBall;
//                skill = new GoToPositionWithOrientation (*closeToBall, angleBallTarget);
                delete skill;
                skill = new Movement::
						GoToPositionWithOrientation (gm->getBallPoint(), angleBallTarget, false);
                goal = gm->getBallPoint();
            }

            break;
        case moveTowardBall:
            cout <<"Move toward the ball"<<endl;

            if(Measurments::isClose(robot->getRobotPosition(), closeToBall, 150)
               && abs(Measurments::angleDiff(robot->getOrientation(), angleBallTarget)) < ANGLE)
            {
                state = driveBall;
                delete skill;
                //reducing speed when robot has the ball in order to keep the ball
                skill = new Movement::GoToPositionWithOrientation (targetPosition, direction, false);
                skill->setVelocityMultiplier(0.4);
            }
            else if(Measurments::distance(goal, gm->getBallPoint()) > CLOSE_ENOUGH)
            // If the ball has changed position
            {
                state = moveBehindBall;
                goal = behindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget,  false);
            }
            break;
        case driveBall:
            robot->setDrible(true);
            cout <<"drive the ball"<<endl;
            // Orientation
            if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 700))
            {
                state = moveBehindBall;
                goal = behindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget, false);
            }
            else if(Measurments::isClose(robot->getRobotPosition(), targetPosition, CLOSE_ENOUGH))
            {
                return true;
            }
            break;
        case haveTheBall: break;
        }

        skill->perform(robot);

        return false;
    }
}
