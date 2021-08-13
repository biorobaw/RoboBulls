#include "robot_pilot.h"
#include "utilities/measurements.h"
#include "path_planning/planners/path_planner_fppa.h"
#include "model/robot/robot.h"
#include "commands/CmdGoToPose.h"
#include "model/team/team.h"
#include "gui/interface/gui_interface.h"
#include "gui/panels/panel_field.h"

#include <QDebug>

RobotPilot::RobotPilot(Robot* robot) : robot(robot), team(robot->getTeam()), game_state(team->getGameState()), planner(new PathPlannerFPPA(robot)) {

}

RobotPilot::~RobotPilot(){
    if(cmdGoToPose != nullptr) {
        delete cmdGoToPose;
        cmdGoToPose = nullptr;
    }
    if(planner!=nullptr) delete planner;
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

        //TODO: ideally we should not recompute the path each time
        auto path = planner->genPath(cmd->target_pose, cmd->avoid_ball);

        GuiInterface *gui = GuiInterface::getGuiInterface();

        //gui->drawLine(*robot, cmd->target_pose);
        gui->drawLine(r_pos, nextPoint);
        Point prev_point = r_pos;
        gui->setHidePaths(false);
        for (auto i : path)
        {
            gui->drawLine(prev_point, i);
            prev_point=i;
           //gui->drawPoint(i);
        }

//        qDebug().nospace() << "P: ";
//        for(auto p : path)//int i=0; i<path.size(); i++)
//            qDebug().nospace() << p << "->";
//        qDebug();
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

