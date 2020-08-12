#include "robot_pilot.h"
#include "utilities/measurements.h"
#include "path_planning/fppa_pathfinding.h"
#include "robot/robot.h"
#include "commands/CmdGoToPose.h"
#include <iostream>
#include "model/team.h"
using std::cout, std::endl;

Pilot::Pilot(Robot* robot) : robot(robot){

}

Pilot::~Pilot(){
    if(cmdGoToPose != nullptr) {
        delete cmdGoToPose;
        cmdGoToPose = nullptr;
    }
}


#define DD if(false)

void Pilot::setNewCommand(CmdGoToPose& newCommand){
    if(cmdGoToPose != nullptr) *cmdGoToPose = newCommand;
    else cmdGoToPose = new CmdGoToPose(newCommand);

    DD cout << "-cmd has angle: " << newCommand.hasTargetAngle() << " " << newCommand.targetAngle << endl;
    if(!cmdGoToPose->hasTargetAngle())
        cmdGoToPose->targetAngle = Measurements::angleBetween(robot, cmdGoToPose->targetPose);
}

void Pilot::cancelCommand(){
    delete cmdGoToPose;
    cmdGoToPose = nullptr;
}

bool Pilot::finishedCommand(){
    return cmdGoToPose == nullptr || cmdGoToPose->completed(robot->getPosition(),robot->getOrientation());
}


bool Pilot::executeCommands(){
    //currently we only support cmdGoToPose, but in the future more commands could be implemented
    if(cmdGoToPose != nullptr) return executeCmdGoToPose(cmdGoToPose); // so far we only support cmdGoToPose

    // if no commands to execute, then 'current action' was already completed
    return true;

}

bool Pilot::executeCmdGoToPose(CmdGoToPose *cmd){
    auto r_pos = robot->getPosition();
    Point nextPoint = cmd->targetPose;
    Point nextNextPoint = nextPoint;

    // do path planning if necessary
    if( ( cmd->avoidObstacles || cmd->avoidBall) &&
            Measurements::distance(r_pos, cmd->targetPose) > cmd->distance_tolerance){

        //std::cout << "---doing path planning\n";

        // Assign robots that are to be considered obstacles
        FPPA::updateRobotObstacles(robot, robot->getTeam()->getGameState());

        //TODO: ideally we should not recompute the path each time
        FPPA::Path path = FPPA::genPath(robot->getTeam()->getGameState(), r_pos, cmd->targetPose, cmd->avoidBall, robot->isGoalie());
        if(path.size()>0){
            nextPoint = path[0];
            nextNextPoint = path[ path.size()>1 ? 1 : 0];
        }
    }

    // drive to the next position
    DD cout << "---next point: (" << nextPoint.x << "," << nextPoint.y << "), "
         << cmd->targetAngle << " , ("
         << nextNextPoint.x  << "," << nextNextPoint.y << ")" << std::endl;
    driveTo(nextPoint, cmd->targetAngle,nextNextPoint);

    // return whether action cempleted or not
    return cmd->completed(r_pos,robot->getOrientation());
}


void Pilot::setTargetVelocity(Point velocity, float angular){
    robot->target_velocity = velocity;
    robot->target_angular_speed = angular;
}
