
#ifndef KICK_H
#define KICK_H

#include "skill/skill.h"
#include "model/robot.h"
#include "communication/robcomm.h"

namespace Skill
{

class Kick: public Skill
{
public:
    Kick();
    Kick(int, int);
    //void kickTheBall();
    bool perform(Robot *);

private:
    bool roboKick;
    int lVel;
    int rVel;

};

}

#endif // KICK_H
