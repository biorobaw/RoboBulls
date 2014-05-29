
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
    //void kickTheBall();
    void perform(Robot *);

private:
    bool roboKick;

};

}

#endif // KICK_H
