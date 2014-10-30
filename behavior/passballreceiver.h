#ifndef PASSBALLRECEIVER_H
#define PASSBALLRECEIVER_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "movement/gotoposition.h"
#include "utilities/point.h"
#include "utilities/paramlist.h"
#include "model/robot.h"
#include "model/gamemodel.h"
#include "behavior/genericmovementbehavior.h"

/*
 * PassBallReceiver class:
 * This class can be used with passBallSender
 * The receiver robot waits for the ball untill
 * the ball gets close it to after it been passed
 * Then the receiver gets behind the ball and
 * shoots toward the goal
 * */

class PassBallReceiver : public GenericMovementBehavior
{
public:
    PassBallReceiver(const ParameterList& list);
    void perform(Robot *);
    void setBallOrigin();
    enum states {initial, moving, approaching, kicking, idling} state;
private:
    Point ballLastSeen;
    Point ballOrg;
    Point target;
    bool hasKicked;
    int count;
    bool detectedBall;
};

#endif // PASSBALLRECEIVER_H
