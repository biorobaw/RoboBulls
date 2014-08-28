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

#define DIST 400
#define CLOSE_ENOUGH 150
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
        GameModel *gm = GameModel::getModel();

        switch(state)
        {
        case initial:
            cout<<"Drive ball initial state"<<endl;
            state = moveBehindBall;
            behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                   DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
            goal = behindBall;
            skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            break;
        case moveBehindBall:
            cout<< "move behind the ball" << endl;

            behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                   DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);


            if (Measurments::distance(*goal, *behindBall) > CLOSE_ENOUGH)
            {
                state = moveBehindBall;
                behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                       DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            if (Measurments::isClose(robot->getRobotPosition(), *behindBall, CLOSE_ENOUGH))
            {
                  state = moveTowardBall;
                  skill = new GoToPositionWithOrientation (gm->getBallPoint(), Measurments::angleBetween(gm->getBallPoint(), targetPosition));
                  *goal = gm->getBallPoint();
            }

            break;
        case moveTowardBall:
            cout <<"Move toward the ball"<<endl;
            if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)) {
                state = driveBall;
                GoToPositionWithOrientation *gotoPos = new GoToPositionWithOrientation (targetPosition, direction);
                //reducing speed when robot has the ball in order to keep the ball
                gotoPos->setVelocityMultiplier(0.5);
                skill = gotoPos;
            }
            // If the ball has changed position
            if(Measurments::distance(*goal, gm->getBallPoint()) > CLOSE_ENOUGH)
            {
                state = moveBehindBall;
                behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                       DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            break;
        case driveBall:
            cout <<"drive the ball"<<endl;
//            cout << "robot & ball dist\t"<< Measurments::distance(robot->getRobotPosition(), gm->getBallPoint()) <<endl;
//            cout << "robot and target dist\t" << Measurments::distance(robot->getRobotPosition(), targetPosition) <<endl;
//            cout << "target and ball dist\t" << Measurments::distance(targetPosition, gm->getBallPoint()) << endl;
//            cout << " angle diff robot and ball\t" <<
//                    Measurments::angleDiff(robot->getOrientation(), Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))/M_PI*180<< endl;
            if(!Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH))
            {
                state = moveBehindBall;
                behindBall = new Point(DIST*cos(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().x,
                                       DIST*sin(Measurments::angleBetween(targetPosition,gm->getBallPoint()))+gm->getBallPoint().y);
                skill = new GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), targetPosition));
            }
            break;
        }

        skill->perform(robot);
        return false;
    }
}
