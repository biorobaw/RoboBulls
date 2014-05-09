#ifndef BEHAVIOR_H
#define BEHAVIOR_H

//#include "Model/robot.h"
//#include "Model/gamemodel.h"

class Robot;
class GameModel;

class Behavior
{
public:
    Behavior();
    virtual void perform(Robot *)=0;

protected:
//    GameModel *gamemodel;

};

#endif // BEHAVIOR_H
