#include "robot_pilot.h"
#include "utilities/measurements.h"
#include "path_planning/fppa_pathfinding.h"
#include "robot/robot.h"
#include "commands/CmdGoToPose.h"
#include "model/team.h"

#include <QDebug>

Pilot::Pilot(Robot* robot) : robot(robot), team(robot->getTeam()), game_state(team->getGameState()){

}

Pilot::~Pilot(){
    if(cmdGoToPose != nullptr) {
        delete cmdGoToPose;
        cmdGoToPose = nullptr;
    }
}


#define DD if(true)

void Pilot::setNewCommand(CmdGoToPose& newCommand){
    if(cmdGoToPose != nullptr) *cmdGoToPose = newCommand;
    else cmdGoToPose = new CmdGoToPose(newCommand);

    if(!cmdGoToPose->hasTargetAngle())
        cmdGoToPose->targetAngle = Measurements::angleBetween(robot, cmdGoToPose->targetPose);

    DD qDebug() << "T: " << newCommand.targetPose
                << newCommand.targetAngle
                << newCommand.avoidBall
                << newCommand.avoidObstacles
                << newCommand.velocity_multiplier;
}

void Pilot::cancelCommand(){
    delete cmdGoToPose;
    cmdGoToPose = nullptr;
}

bool Pilot::finishedCommand(){
    return cmdGoToPose == nullptr || cmdGoToPose->completed(*robot,robot->getOrientation());
}


bool Pilot::executeCommands(){
    //currently we only support cmdGoToPose, but in the future more commands could be implemented
    if(cmdGoToPose != nullptr) return executeCmdGoToPose(cmdGoToPose); // so far we only support cmdGoToPose

    // if no commands to execute, then 'current action' was already completed
    return true;

}

bool Pilot::executeCmdGoToPose(CmdGoToPose *cmd){
    Point r_pos = *robot;
    Point nextPoint = cmd->targetPose;
    Point nextNextPoint = nextPoint;

    // do path planning if necessary
    if( ( cmd->avoidObstacles || cmd->avoidBall) &&
            Measurements::distance(r_pos, cmd->targetPose) > cmd->distance_tolerance){

        qDebug() << "---doing path planning\n";

        // Assign robots that are to be considered obstacles
        FPPA::updateRobotObstacles(robot, game_state);

        //TODO: ideally we should not recompute the path each time
        FPPA::Path path = FPPA::genPath(game_state, r_pos, cmd->targetPose, cmd->avoidBall, robot->isGoalie());

        qDebug().nospace() << "P: ";
        for(auto p : path)//int i=0; i<path.size(); i++)
            qDebug().nospace() << p << "->";
        qDebug();
        if(path.size()>0){
            nextPoint = path[0];
            nextNextPoint = path[ path.size()>1 ? 1 : 0];
        }
    }

    // drive to the next position
    DD qDebug()
         << "---next point:" << nextPoint << cmd->targetAngle << nextNextPoint;
    driveTo(nextPoint, cmd->targetAngle,nextNextPoint);

    // return whether action cempleted or not
    return cmd->completed(r_pos,robot->getOrientation());
}


void Pilot::setTargetVelocity(Point velocity, float angular){
    robot->target_velocity = velocity * cmdGoToPose->velocity_multiplier;
    robot->target_angular_speed = angular * cmdGoToPose->velocity_multiplier;
}
