#ifndef KICK_H
#define KICK_H
#include "../skill.h"

namespace Skill
{

/*! @file
 *  Skill to kick the ball
 *  Sets the kick active on the robot.
 *  The constructor specifies a distance to kick the ball.
 *  <b>(variable distance only implemented on the Simulator)</b> */

class Kick: public Skill
{
public:
    //! A "full-power" default kick distance in points.
    static const int defaultKickDistance = 6000;

public:
    /*! Constructor with optional distance.
     * A kick will be sent with enough velocity to move a stationary
     * ball about \c kickDistance from the kick. Leave blank to send a
     * default-powered kick.
     * @param kickDistance Approximate distance in Points to kick for */
    Kick(float kickDistance = defaultKickDistance);

    bool perform(Robot *);

private:
    /*! Translates a distance request into required initial kick velocity */
    float getVelocityforDistance();

    // Distance we wish to kick
    float kickDistance;
};

}

#endif // KICK_H
