#ifndef HALTSTRATEGY_H
#define HALTSTRATEGY_H

#include "strategy/strategy.h"
#include "skill/skill.h"


class HaltStrategy : public Strategy
{
public:
    HaltStrategy();
    void assignBeh();
    Skill::Skill *s;

};

#endif // HALTSTRATEGY_H
