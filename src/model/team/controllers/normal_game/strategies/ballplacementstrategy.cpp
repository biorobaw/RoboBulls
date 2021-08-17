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
#include <QDebug>

BallPlacementStrategy::BallPlacementStrategy(RobotTeam* _team) : TeamStrategy(_team) {

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
        Point target_location = game_state->getBallPlacement();
        qInfo() << "target location: " <<target_location;
        if(attack1)
        {
            attack1->setBehavior<DribbleToPoint>(target_location);

            std::cout << "attack1 is the kicker" << std::endl;
        }
        else if(attack2)
        {
            attack2->setBehavior<DribbleToPoint>(target_location);

             std::cout << "attack2 is the kicker" << std::endl;
        }
        else if(attack3)
        {
            attack3->setBehavior<DribbleToPoint>(target_location);

             std::cout << "attack3 is the kicker" << std::endl;
        }
        else if(attack4)
        {
            attack4->setBehavior<DribbleToPoint>(target_location);

             std::cout << "attack4 is the kicker" << std::endl;
        }
        else if(wall1)
        {
            wall1->setBehavior<DribbleToPoint>(target_location);

             std::cout << "wall1 is the kicker" << std::endl;
        }
        else if(wall2)
        {
            wall2->setBehavior<DribbleToPoint>(target_location);

             std::cout << "wall2 is the kicker" << std::endl;
        }
        else if(defender1)
        {
            defender1->setBehavior<DribbleToPoint>(target_location);

             std::cout << "defender1 is the kicker" << std::endl;

        for (Robot* rob: team->getRobots())
        {

            if (!rob->hasBehavior() && rob->getRole() != RobotRole::GOALIE){
                rob->setBehavior<RefStop>();
                qInfo() << "beh set to refstop";
            }
        }
        /*if(wall1 && !wall1->hasBehavior())
            wall1->setTargetVelocityLocal(Point(0,0),0);
        if(wall2 && !wall2->hasBehavior())
            wall2->setTargetVelocityLocal(Point(0,0),0);
        if(attack1 && !attack1->hasBehavior())
            attack1->setTargetVelocityLocal(Point(0,0),0);
        if(attack2 && !attack2->hasBehavior())
            attack2->setTargetVelocityLocal(Point(0,0),0);
        if(attack3 && !attack3->hasBehavior())
            attack3->setTargetVelocityLocal(Point(0,0),0);
        if(attack4 && !attack4->hasBehavior())
            attack4->setTargetVelocityLocal(Point(0,0),0);
        if(defender1 && !defender1->hasBehavior())
            defender1->setTargetVelocityLocal(Point(0,0),0);*/

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
}

QString BallPlacementStrategy::getName()
{
    return "Ball Placement";
}


void BallPlacementStrategy::runControlCycle()
{//qInfo()<<"control cycle";
    for (Robot* rob : team->getRobots())
//    {   //if (rob->hasBehavior())
//          //  qInfo() << "Robot id: " << QString::fromStdString(rob->getBehavior()->getName());
        if (rob->hasBehavior() && rob->getBehavior()->getName() == "Dribble to point" && rob->getBehavior()->isFinished()){
            rob->setBehavior<RefStop>();}
//            qInfo() << "beh changed to refstop";
//        }
//    }
//
}
