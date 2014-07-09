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
//    static int gotoBallUpdateFn(Skill::Skill& s, Robot* rob)
//	{
//		s = GoToPosition(GameModel::getModel()->getBallPoint(), DriveBall::direction);
//		return -1;
//	}

//	DriveBall::DriveBall(Point targetPoint, double movingDirection)
//	{
//		Point ballPosition = GameModel::getModel()->getBallPoint();
//		direction = movingDirection;

//		mySkillSequence.addSkill( new GoToPosition(ballPosition,direction), gotoBallUpdateFn);
//		mySkillSequence.addSkill( new GoToPosition(targetPoint,direction) );
//		mySkillSequence.addSkill( new Stop() );
//	}

//	void DriveBall::perform(Robot* robot)
//	{
//		cout << "driving ball" <<endl;

//		mySkillSequence.executeOn(robot);
//	}
    DriveBall::DriveBall(Point targetPoint, double finalDirection)
    {
        targetPosition = targetPoint;
        direction = finalDirection;
        state = initial;
    }

    bool DriveBall::perform(Robot* robot)
    {
//        Point goal(-3000, 0);
        GameModel *gm = GameModel::getModel();

//        Point ball = gm->getBallPoint();
//        Point goal = gm->getOpponentGoal();
//        float targetToBallAngle = Measurments::angleBetween(targetPosition,ball);
//        float ballTotargetAngle = Measurments::angleBetween(ball, targetPosition);
        Point *behindBall;

//        Skill::Skill * skill;
//        switch (state) {
//        case goingBehind:
//            skill = new Skill::GoToPositionWithOrientation(behindBall, ballToGoal);
//            break;
//        case approaching:
//            skill = new Skill::GoToPositionWithOrientation(ball, ballToGoal);
//            break;
//        case kicking:
//            skill = new Skill::Kick();
//            break;
//        }

//        // Perform the skill
//        skill->perform(r);

//        // Evaluate possible transitions
//        switch (state){
//        case goingBehind:
//            if (Measurments::distance(behindBall, r->getRobotPosition()) < 100 && abs(Measurments::angleDiff(r->getOrientation(), ballToGoal)) < 10 * M_PI/180)
//                state = approaching;
//            break;
//        case approaching:
//            if (Measurments::distance(ball, r->getRobotPosition()) < 110 && abs(Measurments::angleDiff(r->getOrientation(), ballToGoal)) < 10 * M_PI/180)
//                state = kicking;
//            else if (Measurments::distance(ball, r->getRobotPosition()) > 200)
//                state = goingBehind;
//            break;
//        case kicking:
//            state = goingBehind;
//        }

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
            if(Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110) /*&&
                    abs(Measurments::angleDiff(robot->getOrientation(), Measurments::angleBetween(gm->getBallPoint(), targetPosition))) < 10 * M_PI/180*/) {
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
