#ifndef STOPBEHAVIOR_H
#define STOPBEHAVIOR_H

#include <iostream>
#include <unordered_map>

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "utilities/paramlist.h"

class GameModel;

/*
 * Stop Behavior
 * All robots position themself one meter away from the ball
 * and face the ball
 * Narges Ghaedi
 */
class StopBehavior : public Behavior
{
public:
    StopBehavior(const ParameterList &list);
	
    void perform(Robot *);

private:
    Point mTargetPoint;
    Point curBallPoint;
};

#endif // STOPBEHAVIOR_H
