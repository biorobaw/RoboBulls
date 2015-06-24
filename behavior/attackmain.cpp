#include <assert.h>
#include "include/config/simulated.h"
#include "skill/kicktopointomni.h"
#include "skill/kicktopoint.h"
#include "attackmain.h"

AttackMain::AttackMain(Robot* attacker)
    : drive_skill(nullptr)
    , pass_skill(nullptr)
    , score_skill(nullptr)
{
    support_attacker = attacker;
    state = initial;
}


AttackMain::~AttackMain()
{
    delete drive_skill;
    delete pass_skill;
    delete score_skill;
}

#if SIMULATED
 #define SCORE_ANGLE_TOLERANCE  10*M_PI/180
 #define PASS_ANGLE_TOLERANCE   6*M_PI/180
#else
 #define SCORE_ANGLE_TOLERANCE  7*M_PI/180
 #define PASS_ANGLE_TOLERANCE   7*M_PI/180
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

    //Create switch logic
    switch (state)
    {
        case initial:
            state = drive;
            drive_skill = new Skill::KickToPoint(gp, -1, shot_distance*1.25);
            break;

        case drive:
            /***************************************************************
             * Evaluate transition to pass state. If the robot if 1000 units away
             * from drive start (check RoboCup rules, this might need to change) this happens.
             */
            if(!Measurments::isClose(drive_start_point, rp, drive_distance))
            {
                //If we have our passer, kick to it. Else kick to goal
                if(support_attacker != nullptr) {
                    delete pass_skill;
                    delete drive_skill;
                    drive_skill = nullptr;
                    pass_skill = new Skill::KickToPoint(&sp);
                    state = pass;
                } else {
                    delete score_skill;
                    Point offset(0, -500 + rand() % 1000);
                    score_skill = new Skill::KickToPoint(gp + offset, SCORE_ANGLE_TOLERANCE);
                    state = score;
                }
            }
            /***************************************************************
             * Evaluate transition to score state. If the robot is within 2250 units
             * of the enemy goal, this happens.
             */
            else if(Measurments::isClose(bp, gp, shot_distance))
            {
                delete score_skill;
                Point offset(0, -500 + rand() % 1000);
                score_skill = new Skill::KickToPoint(gp + offset, SCORE_ANGLE_TOLERANCE);
                state = score;
            }
            else
            {
                drive_skill->perform(robot);
            }
            break;
        #if 1
        case score:
            assert(score_skill != nullptr);

            if(score_skill->perform(robot) == true) {
                done = true;
                state = initial;
            }
            //else if(!Measurments::isClose(bp, gp, shot_distance)) {
            //    state = initial;
            //}
            break;
        #endif
        case pass:
            assert(pass_skill != nullptr);
            if(pass_skill->perform(robot))
                done = true;
            break;
        default: break;
    }
}


bool AttackMain::isFinished()
{
    return done;
}
