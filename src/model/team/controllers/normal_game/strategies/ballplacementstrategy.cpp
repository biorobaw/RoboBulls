#include "ballplacementstrategy.h"
#include "../normal_game_roles.h"
#include <iostream>
#include "model/robot/behaviors/skills/dribbletopoint.h"
#include "model/team/team.h"
#include "model/game_state.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/robot/robot.h"
#include "model/team/controllers/normal_game/behaviors/goalie.h"
#include "model/team/controllers/normal_game/behaviors/refstop.h"
#include "utilities/comparisons.h"
#include <QDebug>

BallPlacementStrategy::BallPlacementStrategy(RobotTeam* _team)
    : TeamStrategy(_team), dribble_CompletionCount(0) {
    cur_state=INSIDE_FIELD; has_ball_ct=0;
    /*for(Robot* robot : team->getRobots())
    {
        robot->clearBehavior();

    }*/
    //dribble_CompletionCount = new int(0);
}

BallPlacementStrategy::~BallPlacementStrategy(){

}


void BallPlacementStrategy::assignBehaviors()
{

    if ((game_state->getRefereeCommand() == 17 && team->getID() == ROBOT_TEAM_BLUE) || (game_state->getRefereeCommand() == 16 && team->getID() == ROBOT_TEAM_YELLOW))
    {
        Robot* wall1 = team->getRobotByRole(RobotRole::DEFEND1);
        Robot* wall2 = team->getRobotByRole(RobotRole::DEFEND2);
        Robot* defender1 = team->getRobotByRole(RobotRole::DEFEND3);
        Robot* attack1 = team->getRobotByRole(RobotRole::ATTACK1);
        Robot* attack2 = team->getRobotByRole(RobotRole::ATTACK2);
        Robot* attack3 = team->getRobotByRole(RobotRole::ATTACK3);
        Robot* attack4 = team->getRobotByRole(RobotRole::ATTACK4);
        //Point* game_state->getBallPlacement() = game_state->getBallPlacement();
        qInfo() << "target location: " <<game_state->getBallPlacement();
        if(attack1)
        {   qInfo()<<"atack1" << game_state->getBallPlacement();
            attack1->setBehavior<DribbleToPoint>(game_state->getBallPlacement(), false, false);

            std::cout << "attack1 is the kicker" << std::endl;
        }
        else if(attack2)
        {
            attack2->setBehavior<DribbleToPoint>(game_state->getBallPlacement());

             std::cout << "attack2 is the kicker" << std::endl;
        }
        else if(attack3)
        {
            attack3->setBehavior<DribbleToPoint>(game_state->getBallPlacement());

             std::cout << "attack3 is the kicker" << std::endl;
        }
        else if(attack4)
        {
            attack4->setBehavior<DribbleToPoint>(game_state->getBallPlacement());

             std::cout << "attack4 is the kicker" << std::endl;
        }
        else if(wall1)
        {
            wall1->setBehavior<DribbleToPoint>(game_state->getBallPlacement());

             std::cout << "wall1 is the kicker" << std::endl;
        }
        else if(wall2)
        {
            wall2->setBehavior<DribbleToPoint>(game_state->getBallPlacement());

             std::cout << "wall2 is the kicker" << std::endl;
        }
        else if(defender1)
        {
            defender1->setBehavior<DribbleToPoint>(game_state->getBallPlacement());

             std::cout << "defender1 is the kicker" << std::endl;

        }

        for (Robot* rob: team->getRobots())
        {
            //qInfo() << rob->hasBehavior() << "Role: " << rob->getRole();
            if ((!rob->hasBehavior()  && rob->getRole() != RobotRole::GOALIE)){
                rob->setBehavior<RefStop>();
                qInfo() << "beh set to refstop";
            }
        }



        Robot* goalie = team->getRobotByRole(RobotRole::GOALIE);
        if(goalie) goalie->setBehavior<Goalie>();


    }
    else
    {
        for(Robot* robot : team->getRobots())
        {
            robot->setTargetVelocityLocal(Point(0,0),0);

        }
    }

}


QString BallPlacementStrategy::getName()
{
    return "Ball Placement";
}


void BallPlacementStrategy::runControlCycle()
{//qInfo() << "enter";
//    if (Comparisons::isPointOutsideField(*game_state->getBall()))
//    for (Robot* rob : team->getRobots())
//        if (rob->hasBehavior() && rob->getBehavior()->getName() == "Dribble to point"){
//            if ((abs(rob->x)-Field::FIELD_LENGTH/2 <ROBOT_RADIUS+50 || abs(rob->y)-Field::FIELD_WIDTH/2 <ROBOT_RADIUS+50))
//            {
//                ((DribbleToPoint*)rob->getBehavior())->setAvoidObstacles(false);
//                cur_state = OUTSIDE_FIELD;
//             }
//         }
//    if (cur_state == OUTSIDE_FIELD)
//        for (Robot* rob : team->getRobots())
//            if (rob->hasBehavior() && rob->getBehavior()->getName() == "Dribble to point"){
//                if(rob->hasBall() && !Comparisons::isPointOutsideField(*rob)){
//                    has_ball_ct++;
//                    if(has_ball_ct>100){
//                        ((DribbleToPoint*)rob->getBehavior())->setAvoidObstacles(true);
//                        cur_state = INSIDE_FIELD;}
//                    }
//                else has_ball_ct= fmin(has_ball_ct-1,0);
//             }

    //mutex.lock();
    for (Robot* rob : team->getRobots())
        if (rob->hasBehavior() && rob->getBehavior()->getName() == "Dribble to point" && rob->getBehavior()->isFinished() ){
        qInfo() << "DTP is finished: " <<rob->getBehavior()->isFinished() << rob->getBehavior();
         ++dribble_CompletionCount;

//        ++(*dribble_CompletionCount);

        if(dribble_CompletionCount > 50){

            rob->setDribble(false);
            rob->setBehavior<RefStop>();}

        }
    //mutex.unlock();
    //qInfo() <<"exit";
}


