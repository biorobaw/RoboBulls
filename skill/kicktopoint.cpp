#include "kicktopoint.h"
#include "kick.h"
#include "movement/gotopositionwithorientation.h"

namespace Skill{

    KickToPoint::KickToPoint(Point Target)
        : move_skill(nullptr)
    {
        target = Target;
        target_tolerance = ROT_TOLERANCE;
        move_skill = new Movement::GoToPosition(Target, UNUSED_ANGLE_VALUE, false);
        state = positioning;
    }

    KickToPoint::KickToPoint(Point Target, float TargetTolerance)
       : move_skill(nullptr)
    {
        target = Target;
        target_tolerance = TargetTolerance;
        move_skill = new Movement::GoToPosition(Target, UNUSED_ANGLE_VALUE, false);
        state = positioning;
    }

    KickToPoint::~KickToPoint()
    {
        delete move_skill;
        move_skill = nullptr;
    }

    /* TODO: I suggest adding a clause to check if the robot already has the ball.
     * If so, do not waste time positioning 
     */
    bool KickToPoint::perform(Robot * robot)
    {
//        #if TRACE
//            cout << "Performing Skill::KickToPoint" << endl;
//        #endif

        GameModel * gm = GameModel::getModel();
        Point bp = gm->getBallPoint();
        Point rp = robot->getRobotPosition();


        float targetToBall = Measurments::angleBetween(target,bp);
        Point behindBall = Point((dist_position)*cos(targetToBall)+bp.x, (dist_position)*sin(targetToBall)+bp.y);
        Point aheadOfBall = Point(-(dist_position)*cos(targetToBall)+bp.x, -(dist_position)*sin(targetToBall)+bp.y);
        float ballToTarget = Measurments::angleBetween(bp, target);

        switch (state)
        {
        case kick:
            //if the robot is close to the ball and facing the target, kick towards target
            {
                ::Skill::Kick k;
                k.perform(robot);
                cout << "KickToPoint: Kicking" << endl;
                return true;
            }
            break;

        case positioning:
            if(abs(Measurments::angleDiff(robot->getOrientation(), ballToTarget)) > target_tolerance)
            {
                if(!Measurments::isClose(rp,bp))
                    move_skill->recreate(behindBall, ballToTarget);
                else
                    move_skill->recreate(rp, ballToTarget, false);

                #if SIMULATED
                    move_skill->setVelocityMultiplier(1);
                #else
                    move_skill->setVelocityMultiplier(0.25);
                #endif
            }
            else
            {
                move_skill->recreate(aheadOfBall, ballToTarget, false);
            }

            move_skill->perform(robot);

            if(Measurments::distance(bp, rp) < dist_kick && abs(Measurments::angleDiff(robot->getOrientation(), ballToTarget)) < target_tolerance)
            {
                std::cout << "Moving to kick" << std::endl;
                state = kick;
            }
            //cout << "KickToPoint: Positioning" << endl;
            break;
        }
        return false;
    }
}
