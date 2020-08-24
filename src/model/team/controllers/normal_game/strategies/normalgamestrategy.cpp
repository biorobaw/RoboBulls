#include "normalgamestrategy.h"
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

NormalGameStrategy::NormalGameStrategy(RobotTeam* _team)
    : TeamStrategy(_team)
    , initialBallPos(*ball)
    , our_def_area(true)
    , opp_def_area(false)
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

void NormalGameStrategy::assignBehaviors()
{
    Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);

    if(wall1)
        wall1->setBehavior<Wall>();
    if(wall2)
        wall2->setBehavior<Wall>();

    assignGoalieIfOk(team);
}


void NormalGameStrategy::runControlCycle()
{
    Robot* deffend1 = team->getRobotByRole(RobotRole::DEFEND1);
    Robot* deffend2 = team->getRobotByRole(RobotRole::DEFEND2);
    Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
    Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);

    Robot* kick_off_rob = deffend1;


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
        if(!Measurements::isClose(initialBallPos, *ball, 50))
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
        if((prev_state != attack && !clearing_ball)
        || (main != nullptr && supp != nullptr && (Measurements::distance(supp,bp) < Measurements::distance(main, bp))))
        {
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
            if(ball_bot != nullptr && ball_bot->getTeamId()!=team->getID())
                state = defend;
        }

        // Evaluate goalkick
        if(prev_state != goalkick)
        {
            if(our_def_area.contains(bp))
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
        std::cout << "Assigning Attack Behaviors" << std::endl;

        // If two attackers available, assign both the one closer
        // to the ball as the main
        if(attack1 != nullptr && attack2 != nullptr)
        {
            if(Measurements::distance(attack1, *ball) <
               Measurements::distance(attack2, *ball))
            {
                attack1->setBehavior<AttackMain>();
                main = attack1;
                attack2->setBehavior<AttackSupport>();
                supp = attack2;
            }
            else
            {
                attack1->setBehavior<AttackSupport>();
                supp = attack1;
                attack2->setBehavior<AttackMain>();
                main = attack2;
            }
        }
        else if(attack1 != nullptr)
        {
            attack1->setBehavior<AttackMain>();
            main = attack1;
            supp = nullptr;
        }
        else if(attack2 != nullptr)
        {
            attack2->setBehavior<AttackMain>();
            main = attack2;
            supp = nullptr;
        }

        prev_state = state;
        state = evaluate;
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
    {
        std::cout << "Assigning Goal Kick Behaviors" << std::endl;

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
void NormalGameStrategy::assignGoalieIfOk(RobotTeam* team)
{
    Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
    if(goalie)
        goalie->setBehavior<Goalie>();
}
//! @endcond

QString NormalGameStrategy::getName(){
    return "normal game";
}
