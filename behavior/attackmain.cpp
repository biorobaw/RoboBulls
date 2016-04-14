#include <assert.h>
#include "include/config/simulated.h"
#include "skill/kicktopointomni.h"
#include "skill/stop.h"
#include "attackmain.h"

AttackMain::AttackMain(Robot* supp_attacker, bool forceGoalKick)
    : forcedGoalKick(forceGoalKick)
    , skill(nullptr)
{
    support_attacker = supp_attacker;
    state = initial;
}

AttackMain::~AttackMain()
{
    delete skill;
}

/* Angle tolerances for kicking in degrees (then converted to radians).
 * Passing is lower because it needs to be more precise */
#if SIMULATED
 #define SCORE_ANGLE_TOLERANCE  (30*M_PI/180)
 #define PASS_ANGLE_TOLERANCE   (15*M_PI/180)
#else
 #define SCORE_ANGLE_TOLERANCE  (7*M_PI/180)
 #define PASS_ANGLE_TOLERANCE   (7*M_PI/180)
#endif

void AttackMain::perform(Robot * robot)
{
    // Get info from gamemodel
    if(support_attacker != nullptr)
        sp = support_attacker->getPosition();
    rp = robot->getPosition();
    gp = gameModel->getOpponentGoal();
    bp = gameModel->getBallPoint();
    goal_direction = Measurements::angleBetween(rp, gp);  //Sets drive direction towards goal

    // When the robot reaches the ball for the first time, drive_start_point is set to current position
    if(touched_ball == false)
    {
        if(Measurements::isClose(rp,bp,250))
        {
            drive_start_point = rp;
            touched_ball = true;
        }
    }

    //Are we far enough (according to SSL rules) from the drive start that we need to kick?
    bool closeToDriveEnd  = touched_ball && !Measurements::isClose(drive_start_point, rp, drive_distance);

    //Create switch logic
    switch (state)
    {
    case initial:
        delete skill;
        skill = new Skill::KickToPointOmni(gp, -1, 0, true);
        state = driving;
        break;
    case driving:
        //So, when we're close to the drive's end...
        if(touched_ball && closeToDriveEnd)
        {
            //If we have a support attacker and are far from a shot AND the supporter is closer
            //to the goal, make a pass. Otherwise kick to goal

            //Is the supp_attacker closer to the goal than we are?
            bool supporterCloserToGoal = Measurements::distance(gp, sp) < Measurements::distance(gp, rp);

            //Is the support attacker reasonably far from us, to prevent stupid short passes?
            bool supporterSlightlyFar = Measurements::distance(support_attacker, robot) > ROB_OBST_DIA*5;

            //Are we close enough to shoot a goal?
            bool closeToGoalShot = Measurements::isClose(bp, gp, shot_distance);

            if( support_attacker != nullptr
                and !forcedGoalKick
                and !closeToGoalShot
                and supporterCloserToGoal
                and supporterSlightlyFar)
            {
                std::cout << "Choosing pass" << std::endl;
                delete skill;
                skill = new Skill::KickToPointOmni(&sp);
            } else {
                std::cout << "Choosing score" << std::endl;
                delete skill;
                Point offset(0, -500 + rand() % 1000);
                skill = new Skill::KickToPointOmni(gp + offset, SCORE_ANGLE_TOLERANCE, -1, true);
            }

            state = end;
        }
    case end:
        /* A state that does nothing. This exists for after we have driven the ball, and we chose
         * to either pass or kick. */
        break;
    default:
        break;
    }

    //Perform whatever skill the above case is telling us
    if(skill != nullptr) {
        if(skill->perform(robot)) {
            done = true;
            state = initial;
        }
    }
}


bool AttackMain::isFinished()
{
    return done;
}
