#include "kicktopoint.h"
#include "kick.h"
#include "gotopositionwithorientation.h"



namespace Skill{

    KickToPoint::KickToPoint(Point Target)
    {
        target = Target;
    }

    KickToPoint::KickToPoint(Point Target, float TargetTolerance)
    {
        target = Target;
        target_tolerance = TargetTolerance;
    }

    bool KickToPoint::perform(Robot * robot)
    {
        #if TRACE
            cout << "Performing Skill::KickToPoint" << endl;
        #endif

        GameModel * gm = GameModel::getModel();
        Point bp = gm->getBallPoint();
        Point rp = robot->getRobotPosition();


        float targetToBall = Measurments::angleBetween(target,bp);
        Point behindBall = Point((dist_position)*cos(targetToBall)+bp.x, (dist_position)*sin(targetToBall)+bp.y);
        float ballToTarget = Measurments::angleBetween(bp, target);
        state = kick;

        switch (state)
        {
        case kick:
            //if the robot is close to the ball and facing the target, kick towards target
            if(Measurments::distance(bp, rp) < dist_kick && abs(Measurments::angleDiff(robot->getOrientation(), ballToTarget)) < target_tolerance)
            {
                skill = new Kick();
                cout << "KickToPoint: Kicking" << endl;
                break;
            }

        case positioning:
            GoToPositionWithOrientation * goToPos = new GoToPositionWithOrientation (behindBall, ballToTarget);
            #if SIMULATED
                goToPos->setVelocityMultiplier(3);
            #else
                goToPos->setVelocityMultiplier(1);
            #endif
            skill = goToPos;
            cout << "KickToPoint: Positioning" << endl;
        }

        skill->perform(robot);

        delete skill;
        return true;
    }
}
