
#ifndef KICK_H
#define KICK_H

#include "skill/skill.h"
#include "Model/robot.h"
#include "communication/robcomm.h"

using namespace std;

//class skill;

class Kick:public Skill
{
public:
    Kick();

//    void kickTheBall();
    void perform(Robot *);

private:
    bool roboKick;

};

#endif // KICK_H
