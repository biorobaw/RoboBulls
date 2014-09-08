#include "communication/robcomm.h"
#include "model/gamemodel.h"
#include "movement/gotopositionwithorientation.h"
#include "skill/stop.h"
#include "skill/skill.h"
#include "math.h"
#include "driveball.h"

#if SIMULATED
    #define CLOSE_ENOUGH 110
    #define ANGLE 13*M_PI/180
    #define DIST 300
    #define VEL_MULT 0.6
#else
    #define CLOSE_ENOUGH 350
    #define ANGLE 10*M_PI/180
    #define DIST 650
    #define VEL_MULT 0.4
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

        behindBall =Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                               DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);

        float angleBallTarget = Measurments::angleBetween(gm->getBallPoint(), targetPosition);

        switch(state)
        {
        case initial:
//            cout<<"Drive ball initial state"<<endl;
            state = moveBehindBall;
            goal = behindBall;
            delete skill;
            skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget);
            break;
        case moveBehindBall:
//            cout<< "move behind the ball" << endl;
            if (Measurments::distance(goal, behindBall) > CLOSE_ENOUGH)
            {
                state = moveBehindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget);
                goal = behindBall;
            }
            else if (Measurments::isClose(robot->getRobotPosition(), behindBall, CLOSE_ENOUGH)
                && abs(Measurments::angleDiff(robot->getOrientation(), angleBallTarget)) < ANGLE)
            {
                state = moveTowardBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (gm->getBallPoint(), angleBallTarget);
                goal = gm->getBallPoint();
            }

            break;
        case moveTowardBall:
//            cout <<"Move toward the ball"<<endl;
            if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH))
            {
                state = driveBall;
                //reducing speed when robot has the ball in order to keep the ball
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (targetPosition, direction, false);
                skill->setVelocityMultiplier(VEL_MULT);
            }
            else if(Measurments::distance(goal, gm->getBallPoint()) > CLOSE_ENOUGH)
            // If the ball has changed position
            {
                state = moveBehindBall;
                goal = behindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget);
            }

            break;
        case driveBall:
//            cout <<"drive the ball"<<endl;
            if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH*2))
            {
                state = moveBehindBall;
                goal = behindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget);
            }
            else if (abs(Measurments::angleDiff(robot->getOrientation(), Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) > ANGLE*3 &&
                     Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH))
            {
                state = moveBehindBall;
                goal = behindBall;
                delete skill;
                skill = new Movement::GoToPositionWithOrientation (behindBall, angleBallTarget);
            }
            break;
        case haveTheBall: break;
        }

        skill->perform(robot);

        return false;

    }
}
