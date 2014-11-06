#include "attackmain.h"
#include "skill/driveball.h"
#include "skill/kicktopoint.h"
#include "behavior/attacksupport.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "skill/kick.h"


AttackMain::AttackMain(const ParameterList& list)
    : drive_skill(nullptr)
    , pass_skill(nullptr)
    , score_skill(nullptr)
{
    Robot* recvBot = list.getParam<Robot*>("recvBot");
    this->support_attacker = recvBot;
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
#if TRACE
    cout << endl << "Performing Behavior::AttackMain" << endl;
#endif

    gm = GameModel::getModel();

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
            touched_ball = true;
        }
    }

    /* New: We will get the support attacker's target, and use KickToPoint
     * to kick to that position via  pointer, so if it changes KTP will not
     * shoot to an old position
     */
    AttackSupport* attackSupt = dynamic_cast<AttackSupport*>
            (support_attacker->getCurrentBeh());
    stp = attackSupt->getCurrentTarget();

    //Create switch logic
    switch (state)
    {
        case initial:
            done = false;
            state = drive;
            drive_skill = new Skill::KickToPoint(gp, ROT_TOLERANCE, shot_distance);
            break;

        case drive:
            /***************************************************************
             * Evaluate transition to pass state. If the robot if 500 units away
             * from drive start (check RoboCup rules, this might need to change) this happens.
             */
            if(!Measurments::isClose(drive_start_point, rp, drive_distance)) 
            {
                delete pass_skill;
                pass_skill = new Skill::KickToPoint(&stp);
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
        #if 1
        case score:
            assert(score_skill != nullptr);

            if(score_skill->perform(robot) == true) {
                done = true;
                state = initial;
            }
            else if(!Measurments::isClose(bp, gp, shot_distance)) {
                state = initial;
            }
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


bool AttackMain::hasKicked()
{
    return done;
}
