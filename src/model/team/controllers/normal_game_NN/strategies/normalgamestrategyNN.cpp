#include "normalgamestrategyNN.h"
#include "../normal_game_roles.h"

//#include "../behaviors/genericmovementbehavior.h"
#include "model/team/controllers/normal_game/behaviors/goalie.h"
#include "model/team/controllers/normal_game/behaviors/attackmain.h"
#include "model/team/controllers/normal_game/behaviors/attacksupport.h"
#include "model/team/controllers/normal_game/behaviors/defendbehavior.h"
#include "model/team/controllers/normal_game/behaviors/refstop.h"
#include "model/team/controllers/normal_game/behaviors/wall.h"
#include "model/team/controllers/normal_game/behaviors/markbot.h"
#include "model/team/controllers/normal_game/behaviors/challengeballbot.h"
#include "model/team/controllers/normal_game/behaviors/penaltygoalie.h"
#include "utilities/comparisons.h"
#include "utilities/edges.h"
#include "model/ball.h"
#include "model/robot/robot.h"
#include "model/game_state.h"
#include "model/team/team.h"
#include "ssl_referee.pb.h"


#include <QDebug>
NormalGameStrategyNN::NormalGameStrategyNN(RobotTeam* _team)
    : NormalGameStrategy(_team)
    , initialBallPos(*ball)
    , our_def_area(false)
    , opp_def_area(true)
{
    auto prev_cmd = game_state->getRefereePreviousCommand();

    // Opp Kick-Off
    if((prev_cmd == Referee_Command_PREPARE_KICKOFF_YELLOW && team->getID() == ROBOT_TEAM_BLUE)
    || (prev_cmd == Referee_Command_PREPARE_KICKOFF_BLUE && team->getID() == ROBOT_TEAM_YELLOW))
        state = opp_kickoff;

    // Our Kick-Off
    else if((prev_cmd == Referee_Command_PREPARE_KICKOFF_BLUE && team->getID() == ROBOT_TEAM_BLUE)
    ||      (prev_cmd == Referee_Command_PREPARE_KICKOFF_YELLOW && team->getID() == ROBOT_TEAM_YELLOW))
        state = our_kickoff_1;

    // We are shooting a penalty
    else if((prev_cmd == Referee_Command_PREPARE_PENALTY_YELLOW && team->getID() == ROBOT_TEAM_YELLOW)
    ||      (prev_cmd == Referee_Command_PREPARE_PENALTY_BLUE && team->getID() == ROBOT_TEAM_BLUE))
            state = shoot_penalty;

    // We are receiving a penalty
    else if((prev_cmd == Referee_Command_PREPARE_PENALTY_BLUE && team->getID() == ROBOT_TEAM_YELLOW)
    ||      (prev_cmd == Referee_Command_PREPARE_PENALTY_YELLOW && team->getID() == ROBOT_TEAM_BLUE))
            state = defend_penalty;

    // Force Start from ref-box. Previous game
    // states ignored.
    else
        state = evaluate;
}

void NormalGameStrategyNN::assignBehaviors()
{
    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* defender1 = team->getRobotByRole(RobotRole::DEFEND3);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);
    Robot* attack3 = team->getRobotByRole(RobotRole::ATTACK3);
    Robot* attack4 = team->getRobotByRole(RobotRole::ATTACK4);

    if(wall1)
        wall1->setBehavior<Wall>();
    if(wall2)
        wall2->setBehavior<Wall>();
    if(attack1)
        attack1->setBehavior<AttackMain>();
    if(attack2)
        attack2->setBehavior<AttackSupport>();
    if(attack3)
        attack3->setBehavior<AttackSupport>();
    if(attack4)
        attack4->setBehavior<AttackSupport>();
    if(defender1)
        defender1->setBehavior<DefendBehavior>();


    assignGoalieIfOk(team);
}


void NormalGameStrategyNN::runControlCycle()
{
    Robot* deffend1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* deffend2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);

    Robot* kick_off_rob = deffend1; // was deffend1


    Point bp = *ball;
    Robot* ball_bot = game_state->getRobotWithBall();

    assignGoalieIfOk(team);

    // Update the flag indicating if the defenders are clearing the ball.
    // This is key for coordination between attack and defense.
    if(deffend1 && deffend2 && deffend1->hasBehavior() && deffend2->hasBehavior())
    {
        clearing_ball = deffend1->getBehavior()->isFinished()
                     || deffend2->getBehavior()->isFinished();
    }
    else if(deffend1 && deffend1->hasBehavior())
        clearing_ball = deffend1->getBehavior()->isFinished();
    else if(deffend2 && deffend2->hasBehavior())
        clearing_ball = deffend2->getBehavior()->isFinished();
    else
        clearing_ball = false;

    // Standard state machine that stays in the "evaluate" state the majority of the time.
    // Most states are for assigning behaviors only when necessary to prevent continuous
    // assignment which breaks behaviors.
    switch(state)
    {
    case our_kickoff_1:
    {
        std::cout << "Kick Off 1" << std::endl;

        // First, one of the defenders positions itself
        // behind the ball and attacker 1
        if(kick_off_rob)
        {
            Point target = bp + Measurements::unitVector(bp-*attack1) * 350;
            float angle = Measurements::angleBetween(*kick_off_rob, *attack1);
            kick_off_rob->setBehavior<GenericMovementBehavior>(target, angle);
            state = our_kickoff_2;
        } else
        {
            attack1->setBehavior<GenericMovementBehavior>(bp, Measurements::angleBetween(*attack1, bp));
            state = evaluate;
        }
    }
        break;
    case our_kickoff_2:
    {
        std::cout << "Kick Off 2" << std::endl;
        // Once the motion is completed, set a new motion command towards attacker 1
        if(kick_off_rob && kick_off_rob->hasBehavior()
        && kick_off_rob->getBehavior()->isFinished())
        {
            float angle = Measurements::angleBetween(kick_off_rob, *attack1);
            kick_off_rob->clearBehavior();
            kick_off_rob->setBehavior<GenericMovementBehavior>(Point(ROBOT_RADIUS-50,0), angle, false, false);
            state = our_kickoff_3;
        }
    }
        break;
    case our_kickoff_3:
    {
        std::cout << "Kick Off 3" << std::endl;
        // The previous actions should nudge the ball towards attacker 1
        // and so we can begin evaluating our options for play
        if(!Measurements::isClose(initialBallPos, *ball, 50) && kick_off_rob)
        {
            kick_off_rob->setBehavior<Wall>();
            state = evaluate;
        }
    }
        break;
    case opp_kickoff:
    {
        std::cout << "Opp Kick Off" << std::endl;
        if(!Measurements::isClose(initialBallPos, *ball, 50))
            state = evaluate;
    }
        break;
    case shoot_penalty:
    {
        std::cout << "Shoot Penalty" << std::endl;
        Robot* shooter = deffend1;
        if (!shooter)
            shooter = attack1;

        if(attack1)
            attack1->setBehavior<AttackSupport>();
        if(shooter)
        {
            shooter->setBehavior<AttackMain>();
            if(dynamic_cast<AttackMain*>(shooter->getBehavior())->hasKickedToGoal())
            {
                shooter->setBehavior<Wall>();
                state = evaluate;
            }
        }
    }
        break;
    case defend_penalty:
    {
        std::cout << "Defend Penalty" << std::endl;
        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
        Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
        goalie->setBehavior<PenaltyGoalie>();

        if(wall1)
            wall1->clearBehavior();
        if(wall2)
            wall2->clearBehavior();

        if(goalie->getBehavior()->isFinished())
        {
            assignGoalieIfOk(team);
            state = evaluate;

            if(wall1)
                wall1->setBehavior<Wall>();
            if(wall2)
                wall2->setBehavior<Wall>();
        }
    }
        break;
    case evaluate:
    {
        //std::cout << "Evaluate" << std::endl;
        // Evaluate attack
        if((prev_state != attack && !clearing_ball && !opp_def_area.contains(bp))
        || (main != nullptr && supp != nullptr && (Measurements::distance(supp,bp) < Measurements::distance(main, bp)))) //this condition is too leniant.
                                                                                                                           //Interupts attack support from recieving
        {
//            if(supp!= nullptr)
//                qInfo() <<"Supp == " << supp->getBehavior()->isFinished();
//            if(supp!=nullptr && main!=nullptr)
//                qInfo() <<"Supp: "<< Measurements::distance(supp,bp) <<" Main: " <<Measurements::distance(main,bp);

            //If we previously assigned attack, but supp is not saying its finished
            //It must not have recieved a pass.
            if(prev_state == attack && supp !=nullptr && !supp->getBehavior()->isFinished())
                ;//wait
            if (ball_bot == nullptr
            || (ball_bot != nullptr && ball_bot->getTeamId() == team->getID()
                                    && ball_bot->getRole() != RobotRole::GOALIE
                                    && ball_bot->getRole() != RobotRole::DEFEND1
                                    && ball_bot->getRole() != RobotRole::DEFEND2))
                state = attack;
        }

        // Evaluate defend
        if(prev_state != defend)
        {
            if(ball_bot != nullptr && ball_bot->getTeamId()!=team->getID()){
                state = defend;
                qInfo() << "Team id" << ball_bot->getTeamId()<< " RObot id: " << ball_bot->getId() << "our id:" << team->getID();
            }
        }

        // Evaluate goalkick
        if(prev_state != goalkick)
        {   //qInfo() << our_def_area.s
            if(opp_def_area.contains(bp))
                state = goalkick;
        }

        // Evaluate clearball
        if(prev_state != clearball)
        {
            if (clearing_ball)
                state = clearball;
        }

    }
        break;
    case attack:
    {
        std::cout << "Assigning Attack Behaviors" << std::endl; // this statement before the code exits

        // If two attackers available, assign both the one closer
        // to the ball as the main
        if(attack1 != nullptr && attack2 != nullptr)
        {
            if(Measurements::distance(attack1, *ball) <
               Measurements::distance(attack2, *ball)) //If one robot is closer to ball than the
            {                                         //other, assign that to attack main
                //std::cout << "start"<< std::endl;
                attack1->setBehavior<AttackMain>(); // attackmain is the problem

                main = attack1;
                 //std::cout << "end"<< std::endl;
                attack2->setBehavior<AttackSupport>();
                supp = attack2;

            }
            else
            {
                //std::cout << "start1"<< std::endl;
                attack1->setBehavior<AttackSupport>();
                supp = attack1;
                //std::cout << "end1"<< std::endl;
                attack2->setBehavior<AttackMain>();
                main = attack2;

            }
        }
        else if(attack1 != nullptr)
        {
            //std::cout << "start2"<< std::endl;
            attack1->setBehavior<AttackMain>();
            main = attack1;
            supp = nullptr;
            //std::cout << "end2"<< std::endl;
        }
        else if(attack2 != nullptr)
        {
            //std::cout << "start3"<< std::endl;
            attack2->setBehavior<AttackMain>();
            main = attack2;
            supp = nullptr;
            //std::cout << "end3"<< std::endl;
        }
//std::cout << "start4"<< std::endl;
        prev_state = state;
        state = evaluate;
        //std::cout << "end4"<< std::endl;
    }
        break;
    case defend:
    {
        std::cout << "Assigning Defend Behaviors" << std::endl;

        if(attack1)
            attack1->setBehavior<MarkBot>();
        if(attack2)
            attack2->setBehavior<ChallengeBallBot>();

        prev_state = state;
        state = evaluate;
    }
        break;
    case goalkick:
    {   //this is getting trigger when in opponents goal area.
        std::cout << "Assigning Goal Kick Behaviors" << std::endl;
        //if(ball_bot!=nullptr)
        //    qInfo() << "Robot with ball: "<<ball_bot->getId() << "Team" <<ball_bot->getTeamId();


        if(attack1)
            attack1->setBehavior<MarkBot>();
        if(attack2)
            attack2->setBehavior<AttackSupport>();

        prev_state = state;
        state = evaluate;
    }
        break;
    case clearball:
    {
        std::cout << "Assigning Clear Behaviors" << std::endl;

        if(attack1)
            attack1->setBehavior<AttackSupport>();
        if(attack2)
            attack2->setBehavior<AttackSupport>();

        prev_state = state;
        state = evaluate;
    }
        break;
    }

}

// Utility function to not duplicate this code
void NormalGameStrategyNN::assignGoalieIfOk(RobotTeam* team)
{
    Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
    if(goalie)
        goalie->setBehavior<Goalie>();
}
//! @endcond

QString NormalGameStrategyNN::getName(){
    return "normal game";
}

int NormalGameStrategyNN::getStatus(){
    return state;
}
