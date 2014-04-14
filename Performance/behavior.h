#ifndef BEHAVIOR_H
#define BEHAVIOR_H

//#include "Robot/robot.h"
//#include "Model/gamemodel.h"

class Robot;
class GameModel;

class Behavior
{
public:
    Behavior();
    virtual void perform(Robot *);

protected:
//    GameModel *gamemodel;

};

#endif // BEHAVIOR_H
