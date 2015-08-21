#ifndef KICK_H
#define KICK_H
#include "skill/skill.h"

namespace Skill
{

/*! @brief Skill to kick the ball
 *  @author Narges Ghaedi
 *  @details Sets the kick to 1 on the robot.
 *  The constructors can emulate different kick distances
 *  by sending velocities to the while kicking. See Ctors */

class Kick: public Skill
{
public:
    //! @brief Default constructor; wheel velocities normal
    Kick();

    /*! @brief Custom velocity constructor
     * @param lvel velocity to send to the left wheel when kicking
     * @param lvel velocity to send to the left wheel when kicking */
    Kick(int lvel, int rvel);

    bool perform(Robot *);

private:
    int lVel;
    int rVel;
};

}

#endif // KICK_H
