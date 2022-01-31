#include "dummy.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

#include <QDebug>



Dummy::Dummy(Robot* robot) : Behavior(robot)
{
finished = true;
}

bool Dummy::perform()
{
    finished = Measurements::distance(*robot, Target)<50;
    //qInfo() <<Measurements::distance(*robot, Target);

    if(finished){
            finished = false; sign*=-1;
    }

    if(!finished){

    Target = Point(robot->x, sign*Field::GOAL_LENGTH/(4));
    //qInfo() << Target;
    auto cmd = CmdGoToPose(Target,robot->getOrientation(),false,false);
    cmd.velocity_multiplier = 1.0;
    robot->goToPose(cmd);
    }

    return isFinished();
}
Dummy::~Dummy()
{
}
bool Dummy::isFinished()
{
    return finished;
}
string Dummy::getName() {
    return "Goalie"; //so it can be in goal
}
