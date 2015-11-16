#ifndef PASSBALLRECEIVER_H
#define PASSBALLRECEIVER_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "model/robot.h"
#include "model/gamemodel.h"
#include "behavior/genericmovementbehavior.h"

/*! @brief Skill to recieve a ball pass from PassBallSender
 * @author Narges Ghaedi
 * @details This class can be used with passBallSender.
 * Once the sender passes the ball, an int count = 170
 * would start to count down.
 * The receiver robot waits for the ball until
 * the ball gets close to it or after the count becomes 0.
 * With differential robots, the kicking is not very
 * powerful. Hence, we use a counter. The counter starts
 * counting down after the sender passes the ball.
 * If the ball does not get close to receiver before count
 * become 0, the receiver moves toward the ball since
 * it assumes that the ball has stopped somewhere far from it.
 * The receiver gets behind the ball and
 * shoots toward the goal
 * @see PassBallSender */

class PassBallReceiver : public StaticMovementBehavior
{
public:
    PassBallReceiver(Robot *theSender);
    ~PassBallReceiver();
    void perform(Robot *);
    bool isFinished() override;

    bool playerInBadArea(Robot*);
    Point getPasserPassPoint();

private:
    Robot* passer;  //Pointer to passer robot
    enum states {initial, kicking, idling} state;
    Point goalArea;    //Dynamic goal point to kick to
    Skill::Skill *kickToPoint;
};

#endif // PASSBALLRECEIVER_H
