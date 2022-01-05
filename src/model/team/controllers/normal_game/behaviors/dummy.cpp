#include "dummy.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

#include <QDebug>



Dummy::Dummy(Robot* robot) : Behavior(robot)
{

}

bool Dummy::perform()
{
    finished = Measurements::distance(*robot, Target)<300;
    //qInfo() <<Measurements::distance(*robot, Target);

    if(finished){
            finished = false; sign*=-1;
    }

    if(!finished){
    Target = Point(robot->x, sign*Field::FIELD_WIDTH/2);
    auto cmd = CmdGoToPose(Target,robot->getOrientation(),true,false);
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
    return "Dummy";
}
