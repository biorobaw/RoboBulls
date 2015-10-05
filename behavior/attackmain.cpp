#include <assert.h>
#include "include/config/simulated.h"
#include "skill/kicktopointomni.h"
#include "skill/stop.h"
#include "attackmain.h"

AttackMain::AttackMain(Robot* attacker)
    : skill(nullptr)
{
    support_attacker = attacker;
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
    gm = GameModel::getModel();

    //Get info from gamemodel
    if(support_attacker != nullptr)
        sp = support_attacker->getRobotPosition();
    rp = robot->getRobotPosition();
    gp = gm->getOpponentGoal();
    bp = gm->getBallPoint();
    goal_direction = Measurments::angleBetween(rp, gp);  //Sets drive direction towards goal

    //When the robot reaches the ball for the first time, drive_start_point is set to current position
    if(touched_ball == false)
    {
        if(Measurments::isClose(rp,bp,250))
        {
            drive_start_point = rp;
            touched_ball = true;
        }
    }

    //Are we close enough to shoot a goal?
    bool closeToGoalShot = Measurments::isClose(bp, gp, shot_distance);

    //Are we far from (according to rules) from the drive start that we need to kick?
    bool closeToDriveEnd  = touched_ball && !Measurments::isClose(drive_start_point, rp, drive_distance);

    //Create switch logic
    switch (state)
    {
    case initial:
        //std::cout << "Initial" << std::endl;
        delete skill;
        skill = new Skill::KickToPointOmni(gp, -1, 0, true);
        state = driving;
        break;
    case driving:
        if(touched_ball && closeToDriveEnd)
        {
            //If we have our passer and are far from a shot, and the supporter is closer
            //to the goal, make a pass. Otherwise kick to goal
            bool supporterCloserToGoal =
                    Measurments::distance(gp, sp) < Measurments::distance(gp, rp);
            if((support_attacker != nullptr) && !closeToGoalShot && supporterCloserToGoal)
            {
                //std::cout << "Choosing pass" << std::endl;
                delete skill;
                skill = new Skill::KickToPointOmni(&sp, 0.25);
            } else {
                //std::cout << "Choosing score" << std::endl;
                Point offset(0, -500 + rand() % 1000);
                delete skill;
                skill = new Skill::KickToPointOmni(gp + offset, SCORE_ANGLE_TOLERANCE, -1, true);
            }
            state = end;
        }
    case end:
        /* A state that does nothing. This exists for after we have driven the ball, and we chose
         * to either pass or kick. We're just performing the skill at this point */
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
