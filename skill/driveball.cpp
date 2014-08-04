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

#define DIST 100
#define CLOSE_ENOUGH 200
#define ANGLE 20*M_PI/180

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
        #if TRACE
            cout << "Performing Skill::DriveBall" << endl;
        #endif

        GameModel *gm = GameModel::getModel();

        switch(state)
        {
        case initial:
            //cout<<"Drive ball initial state"<<endl;
            state = moveTowardBall;
            behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                   DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
            skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            break;
        case moveTowardBall:
            //cout <<"Move toward the ball"<<endl;
            if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)
                    && abs(Measurments::angleDiff(robot->getOrientation(), Measurments::angleBetween(gm->getBallPoint(), targetPosition))) <= ANGLE) {
                state = driveBall;
                GoToPositionWithOrientation *gotoPos = new GoToPositionWithOrientation (targetPosition, direction);
                //reducing speed when robot has the ball in order to keep the ball
                gotoPos->setVelocityMultiplier(0.7);
                skill = gotoPos;
            }
            // If the ball has changed position
            else if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)){
                state = moveTowardBall;
                behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                       DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            break;
        case driveBall:
            //cout <<"drive the ball"<<endl;
            if(Measurments::isClose(robot->getRobotPosition(), targetPosition, CLOSE_ENOUGH)) {
                state = idiling;

            }
            else if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)) {
                state = moveTowardBall;
                behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                       DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }

            break;
        case idiling:
            cout<<"stoping"<<endl;
            if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)) {
                state = moveTowardBall;
                behindBall = new Point(150*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                       150*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            else if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)
                    && !Measurments::isClose(robot->getRobotPosition(), targetPosition, CLOSE_ENOUGH)
                    && abs(Measurments::angleDiff(robot->getOrientation(), Measurments::angleBetween(gm->getBallPoint(), targetPosition))) <= ANGLE)
            {
                state = driveBall;
                skill = new GoToPositionWithOrientation (targetPosition, direction);
            }
            break;
        }

        skill->perform(robot);
// delete skill;
// delete behindBall;

        return false;
    }
}
