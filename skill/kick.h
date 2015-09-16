#ifndef KICK_H
#define KICK_H
#include "skill/skill.h"

namespace Skill
{

/*! @brief Skill to kick the ball
 *  @author Narges Ghaedi, JamesW
 *  @details Sets the kick active on the robot.
 *  The constructor specifies a distance to kick the ball.
 *  <b>(Only implemented on the Simulator)</b> */

class Kick: public Skill
{
public:
    //! @brief A "full-power" default kick distance in points.
    static const int defaultKickDistance = 6000;

public:
    /*! @brief Constructor with optional distance
     * @details A kick will be sent with enough
     * velocity to move a stationary ball about kickDistance
     * from the kick. Leave blank to send a default-powered kick.
     * @param kickDistance Approximate distance in Points to kick for */
    Kick(float kickDistance = defaultKickDistance);

    bool perform(Robot *);

private:
    //Translates a distance request into required initial kick velocity
    float getVelocityforDistance();

    //Distance we wish to kick
    float kickDistance;
};

}

#endif // KICK_H
