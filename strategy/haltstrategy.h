#ifndef HALTSTRATEGY_H
#define HALTSTRATEGY_H
#include "strategy/strategy.h"
#include "skill/skill.h"

/*! @brief Strategy to implement the Halt game state 'H'
 *  @author Narges G */

class HaltStrategy : public Strategy
{
public:
    HaltStrategy();
    void assignBeh();
    Skill::Skill *s;

};

#endif // HALTSTRATEGY_H
