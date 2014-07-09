#include "driveball.h"
#include "communication/robcomm.h"
#include "model/gamemodel.h"
#include "skill/gotopositionwithorientation.h"
#include "skill/basic_movement.h"
#include "include/globals.h"
#include "skill/skill.h"
#include "utilities/skillsequence.h"
#include "skill/differential_control/closedloopcontrol.h"

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
        GameModel *gm = GameModel::getModel();
        Point *behindBall;

        switch(state)
        {
        case initial:
            cout<<"Drive ball initial state"<<endl;
            state = moveTowardBall;
            behindBall = new Point(50*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x, 50*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
            skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            break;
        case moveTowardBall:
            cout <<"Move toward the ball"<<endl;
            if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110)) {
                state = driveBall;
                skill = new GoToPositionWithOrientation (targetPosition, direction);
            }
            else if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110)) {
                state = moveTowardBall;
                skill = new GoToPositionWithOrientation (gm->getBallPoint(), Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            break;
        case driveBall:
            cout <<"drive the ball"<<endl;
            if(Measurments::isClose(robot->getRobotPosition(), targetPosition, 110)) {
                state = idiling;
                skill = new Stop();
            }
            else if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110)) {
                state = moveTowardBall;
                behindBall = new Point(50*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x, 50*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            break;
        case idiling:
            cout<<"stoping"<<endl;
            if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110)) {
                state = moveTowardBall;
                behindBall = new Point(50*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x, 50*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            else if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110) && !Measurments::isClose(robot->getRobotPosition(), targetPosition, 110))
            {
                state = driveBall;
                skill = new GoToPositionWithOrientation (targetPosition, direction);
            }
            break;
        }

        skill->perform(robot);

		
        return false;
    }
}
