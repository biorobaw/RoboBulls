#include "robot_pilot.h"
#include "utilities/measurements.h"
#include "path_planning/fppa_pathfinding.h"
#include "model/robot/robot.h"
#include "commands/CmdGoToPose.h"
#include "model/team/team.h"

#include <QDebug>

RobotPilot::RobotPilot(Robot* robot) : robot(robot), team(robot->getTeam()), game_state(team->getGameState()){

}

RobotPilot::~RobotPilot(){
    if(cmdGoToPose != nullptr) {
        delete cmdGoToPose;
        cmdGoToPose = nullptr;
    }
}


#define DD if(false)

void RobotPilot::setNewCommand(CmdGoToPose& newCommand){
    if(cmdGoToPose != nullptr) *cmdGoToPose = newCommand;
    else cmdGoToPose = new CmdGoToPose(newCommand);

    if(!cmdGoToPose->hasTargetAngle())
        cmdGoToPose->target_angle = Measurements::angleBetween(robot, cmdGoToPose->target_pose);

    DD qDebug() << "T: " << newCommand.target_pose
                << newCommand.target_angle
                << newCommand.avoid_ball
                << newCommand.avoid_obstacles
                << newCommand.velocity_multiplier;
}

void RobotPilot::cancelCommand(){
    delete cmdGoToPose;
    cmdGoToPose = nullptr;
}

bool RobotPilot::finishedCommand(){
    return cmdGoToPose == nullptr || cmdGoToPose->completed(*robot,robot->getOrientation());
}


bool RobotPilot::executeCommands(){
    //currently we only support cmdGoToPose, but in the future more commands could be implemented
    if(cmdGoToPose != nullptr) return executeCmdGoToPose(cmdGoToPose); // so far we only support cmdGoToPose

    // if no commands to execute, then 'current action' was already completed
    return true;

}

bool RobotPilot::executeCmdGoToPose(CmdGoToPose *cmd){
    Point r_pos = *robot;
    Point nextPoint = cmd->target_pose;
    Point nextNextPoint = nextPoint;

    // do path planning if necessary
    if( ( cmd->avoid_obstacles || cmd->avoid_ball) &&
            Measurements::distance(r_pos, cmd->target_pose) > cmd->distance_tolerance){

//        qDebug() << "---doing path planning\n";

        // Assign robots that are to be considered obstacles
        FPPA::updateRobotObstacles(robot, game_state);

        //TODO: ideally we should not recompute the path each time
        FPPA::Path path = FPPA::genPath(game_state, r_pos, cmd->target_pose, cmd->avoid_ball, robot->isGoalie());

//        qDebug().nospace() << "P: ";
        for(auto p : path)//int i=0; i<path.size(); i++)
//            qDebug().nospace() << p << "->";
        qDebug();
        if(path.size()>0){
            nextPoint = path[0];
            nextNextPoint = path[ path.size()>1 ? 1 : 0];
        }
    }

    // drive to the next position
    DD qDebug()
         << "---next point:" << nextPoint << cmd->target_angle << nextNextPoint;
    driveTo(nextPoint, cmd->target_angle,nextNextPoint);

    // return whether action cempleted or not
    return cmd->completed(r_pos,robot->getOrientation());
}

