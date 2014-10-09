#include "attackmain.h"
#include "skill/driveball.h"
#include "skill/kicktopoint.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "skill/kick.h"


AttackMain::AttackMain(const ParameterList& list)
    : drive_skill(nullptr)
    , pass_skill(nullptr)
    , score_skill(nullptr)
{
    UNUSED_PARAM(list);
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
 #define PASS_ANGLE_TOLERANCE   4*M_PI/180
#else
 #define SCORE_ANGLE_TOLERANCE  7*M_PI/180
 #define PASS_ANGLE_TOLERANCE   7*M_PI/180
#endif

void AttackMain::perform(Robot * robot)
{
#if TRACE
    cout << endl << "Performing Behavior::AttackMain" << endl;
#endif

    gm = GameModel::getModel();

    //Set either robID(1) or robID(2) as the support_attacker
    Robot * support_attacker;

    /* Comment: This may not be the correct way to do this.
     * the team vectors in gameModel are not guaranteed to be
     * indexed by ID. It would be proper to use GameModel::find,
     * or later I suggest sorting the team vectors at the beginning
     * of each game loop
     */
    if (robot->getID()==0)
        support_attacker = gm->getMyTeam().at(1);
    else
        support_attacker = gm->getMyTeam().at(0);


    //Get info from gamemodel
    sp = support_attacker->getRobotPosition();
    rp = robot->getRobotPosition();
    gp = gm->getOpponentGoal();
    bp = gm->getBallPoint();
    goal_direction = Measurments::angleBetween(rp, gp);  //Sets drive direction towards goal

    //When the robot reaches the ball for the first time, drive_start_point is set to current position
    if(touched_ball == false)
    {
        drive_start_point = rp;
        if(Measurments::isClose(rp,bp,150))
        {
            #if SIMULATED
                touched_ball = true;    /* Why is this only done when simulated? */
            #endif
        }
    }

    //Create switch logic
    switch (state)
    {
        case initial:
            state = drive;
            delete drive_skill;
            drive_skill = new Skill::DriveBall(gp, goal_direction);
            break;

        case drive:
            /***************************************************************
             * Evaluate transition to pass state. If the robot if 500 units away
             * from drive start (check RoboCup rules, this might need to change) this happens.
             */
            if(!Measurments::isClose(drive_start_point, rp, drive_distance)) 
            {
                delete pass_skill;
                pass_skill = new Skill::KickToPoint(sp, PASS_ANGLE_TOLERANCE);
                state = pass;
            }
            /***************************************************************
             * Evaluate transition to score state. If the robot is within 1500 units
             * of the enemy goal, this happens.
             */
            else if(Measurments::isClose(bp, gp, shot_distance)) 
            {
                delete score_skill;
                score_skill = new Skill::KickToPoint(gp, SCORE_ANGLE_TOLERANCE);
                state = score;
            }
            else  
            {
                drive_skill->perform(robot);
            }
            break;
            
        case score:
            assert(score_skill != nullptr);

            /* Shamsi change this later; what happens after
             * the robot kicks the ball in the score state? Right
             * now I just had them go back to initial
             */
            if(score_skill->perform(robot) == true) {
                state = initial;
            }
            else if(!Measurments::isClose(bp, gp, shot_distance)) {
                state = initial;
            }
            break;
            
        case pass:
            assert(pass_skill != nullptr);
            pass_skill->perform(robot);
            break;
    }
}
