#include "include/config/team.h"
#include "behavior/genericmovementbehavior.h"
#include "behavior/goalie.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/defendbehavior.h"
#include "behavior/refstop.h"
#include "behavior/wall.h"
#include "behavior/markbot.h"
#include "behavior/challengeballbot.h"
#include "behavior/penaltygoalie.h"
#include "utilities/comparisons.h"
#include "utilities/edges.h"
#include "strategy/normalgamestrategy.h"

NormalGameStrategy::NormalGameStrategy()
    : initialBallPos(GameModel::getModel()->getBallPoint())
    , our_def_area(OUR_TEAM)
    , opp_def_area(!OUR_TEAM)
{
    char prevGs = gameModel->getPreviousGameState();

    // Opp Kick-Off
    if((prevGs == 'k' && OUR_TEAM == TEAM_BLUE)
    || (prevGs == 'K' && OUR_TEAM == TEAM_YELLOW))
        state = opp_kickoff;

    // Our Kick-Off
    else if((prevGs == 'K' && OUR_TEAM == TEAM_BLUE)
    ||      (prevGs == 'k' && OUR_TEAM == TEAM_YELLOW))
        state = our_kickoff_1;

    // We are shooting a penalty
    else if((prevGs == 'p' && OUR_TEAM == TEAM_YELLOW)
    ||      (prevGs == 'P' && OUR_TEAM == TEAM_BLUE))
            state = shoot_penalty;

    // We are receiving a penalty
    else if((prevGs == 'P' && OUR_TEAM == TEAM_YELLOW)
    ||      (prevGs == 'p' && OUR_TEAM == TEAM_BLUE))
            state = defend_penalty;

    // Force Start from ref-box. Previous game
    // states ignored.
    else
        state = evaluate;
}

void NormalGameStrategy::assignBeh()
{
    Robot* wall1 = gameModel->findMyTeam(DEFEND_1);
    Robot* wall2 = gameModel->findMyTeam(DEFEND_2);

    if(wall1)
        wall1->assignBeh<Wall>();
    if(wall2)
        wall2->assignBeh<Wall>();

    assignGoalieIfOk();
}


bool NormalGameStrategy::update()
{
    Robot* attack1 = gameModel->findMyTeam(ATTACK_1);
    Robot* attack2 = gameModel->findMyTeam(ATTACK_2);
    Robot* kick_off_rob = gameModel->findMyTeam(DEFEND_1);

    Point bp = gameModel->getBallPoint();
    Robot* ball_bot = gameModel->getHasBall();

    assignGoalieIfOk();

    // Update the flag indicating if the defenders are
    // clearing the ball. Key for coordination between attack
    // and defense.
    if(gameModel->findMyTeam(DEFEND_1) && gameModel->findMyTeam(DEFEND_1)->hasBehavior()
    && gameModel->findMyTeam(DEFEND_2) && gameModel->findMyTeam(DEFEND_2)->hasBehavior())
    {
        clearing_ball = gameModel->findMyTeam(DEFEND_1)->getBehavior()->isFinished()
                     || gameModel->findMyTeam(DEFEND_2)->getBehavior()->isFinished();
    }
    else if(gameModel->findMyTeam(DEFEND_1) && gameModel->findMyTeam(DEFEND_1)->hasBehavior())
        clearing_ball = gameModel->findMyTeam(DEFEND_1)->getBehavior()->isFinished();
    else if(gameModel->findMyTeam(DEFEND_2) && gameModel->findMyTeam(DEFEND_2)->hasBehavior())
        clearing_ball = gameModel->findMyTeam(DEFEND_2)->getBehavior()->isFinished();
    else
        clearing_ball = false;

    // Standard state machine that stays in the "evaluate" state
    // the majority of the time. Most states are for assigning behaviors
    // only when necessary to prevent continuous assignment which breaks
    // behaviors.
    switch(state)
    {
    case our_kickoff_1:
    {
        std::cout << "Kick Off 1" << std::endl;

        // First, one of the defenders positions itself
        // behind the ball and attacker 1
        if(kick_off_rob)
        {
            Point target = bp + Measurements::unitVector(bp-attack1->getPosition()) * 350;
            float angle = Measurements::angleBetween(kick_off_rob->getPosition(), attack1->getPosition());
            kick_off_rob->assignBeh<GenericMovementBehavior>(target, angle);
            state = our_kickoff_2;
        }
    }
        break;
    case our_kickoff_2:
    {
        std::cout << "Kick Off 2" << std::endl;
        // Once the motion is completed, set a new motion command
        // towards attacker 1
        if(kick_off_rob && kick_off_rob->hasBehavior()
        && kick_off_rob->getBehavior()->isFinished())
        {
            float angle = Measurements::angleBetween(kick_off_rob->getPosition(), attack1->getPosition());
            kick_off_rob->clearBehavior();
            kick_off_rob->assignBeh<GenericMovementBehavior>(Point(ROBOT_RADIUS-50,0), angle, false, false);
            state = our_kickoff_3;
        }
    }
        break;
    case our_kickoff_3:
    {
        std::cout << "Kick Off 3" << std::endl;
        // The previous actions should nudge the ball towards attacker 1
        // and so we can begin evaluating our options for play
        if(!Measurements::isClose(initialBallPos, gameModel->getBallPoint(), 50))
        {
            kick_off_rob->assignBeh<Wall>();
            state = evaluate;
        }
    }
        break;
    case opp_kickoff:
    {
        std::cout << "Opp Kick Off" << std::endl;
        if(!Measurements::isClose(initialBallPos, gameModel->getBallPoint(), 50))
            state = evaluate;
    }
        break;
    case shoot_penalty:
    {
        std::cout << "Shoot Penalty" << std::endl;
        Robot* shooter = gameModel->findMyTeam(DEFEND_1);
        if(shooter)
        {
            shooter->assignBeh<AttackMain>();
            if(dynamic_cast<AttackMain*>(shooter->getBehavior())->hasKickedToGoal())
            {
                shooter->assignBeh<Wall>();
                state = evaluate;
            }
        }
    }
        break;
    case defend_penalty:
    {
        std::cout << "Defend Penalty" << std::endl;
        Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
        Robot* wall1 = gameModel->findMyTeam(DEFEND_1);
        Robot* wall2 = gameModel->findMyTeam(DEFEND_2);
        goalie->assignBeh<PenaltyGoalie>();

        if(wall1)
            wall1->clearBehavior();
        if(wall2)
            wall2->clearBehavior();

        if(goalie->getBehavior()->isFinished())
        {
            assignGoalieIfOk();
            state = evaluate;

            if(wall1)
                wall1->assignBeh<Wall>();
            if(wall2)
                wall2->assignBeh<Wall>();
        }
    }
        break;
    case evaluate:
    {
        std::cout << "Evaluate" << std::endl;
        // Evaluate attack
        if((prev_state != attack && !clearing_ball)
        || (main != nullptr && supp != nullptr && (Measurements::distance(supp,bp) < Measurements::distance(main, bp))))
        {
            if (ball_bot == nullptr
            || (ball_bot != nullptr && ball_bot->isOnMyTeam()
                                    && ball_bot->getID() != GOALIE_ID
                                    && ball_bot->getID() != DEFEND_1
                                    && ball_bot->getID() != DEFEND_2))
                state = attack;
        }

        // Evaluate defend
        if(prev_state != defend)
        {
            if(ball_bot != nullptr && !ball_bot->isOnMyTeam())
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
            if(Measurements::distance(attack1, gameModel->getBallPoint()) <
               Measurements::distance(attack2, gameModel->getBallPoint()))
            {
                attack1->assignBeh<AttackMain>();
                main = attack1;
                attack2->assignBeh<AttackSupport>();
                supp = attack2;
            }
            else
            {
                attack1->assignBeh<AttackSupport>();
                supp = attack1;
                attack2->assignBeh<AttackMain>();
                main = attack2;
            }
        }
        else if(attack1 != nullptr)
        {
            attack1->assignBeh<AttackMain>();
            main = attack1;
            supp = nullptr;
        }
        else if(attack2 != nullptr)
        {
            attack2->assignBeh<AttackMain>();
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
            attack1->assignBeh<MarkBot>();
        if(attack2)
            attack2->assignBeh<ChallengeBallBot>();

        prev_state = state;
        state = evaluate;
    }
        break;
    case goalkick:
    {
        std::cout << "Assigning Goal Kick Behaviors" << std::endl;

        if(attack1)
            attack1->assignBeh<MarkBot>();
        if(attack2)
            attack2->assignBeh<AttackSupport>();

        prev_state = state;
        state = evaluate;
    }
        break;
    case clearball:
    {
        std::cout << "Assigning Clear Behaviors" << std::endl;

        if(attack1)
            attack1->assignBeh<AttackSupport>();
        if(attack2)
            attack2->assignBeh<AttackSupport>();

        prev_state = state;
        state = evaluate;
    }
        break;
    }

    return false;
}

// Utility function to not duplicate this code
void NormalGameStrategy::assignGoalieIfOk()
{
    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<Goalie>();
}
//! @endcond
