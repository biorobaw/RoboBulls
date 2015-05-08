#ifndef JOYSTICK_H

/* JOYSTICK SUPPORT
 * Uses SDL to interface with a single connected joystick to control a robot.
 * From `listen`, the output wheel velocities are passed. Use these to control
 * a robot from Ryan's GUI if it is selected.
 *
 * Currently supported joysticks:
 * - Saitek P990 Dual Analog Pad
 * -
 */

namespace joystick
{

//Wheel velocities and Kick calculated from Joystick movement.
//Read these and send them to a robot.
extern float LB, LF, RB, RF;
extern bool  Kick, Dribble;

/********************************/

//Initializes joystick support. *Call this before any other calls*
void init();

//Returns true if there is a joystick connected to the system and stores
//the result (see hasSupport)
bool checkForJoystick();

//Can be used to query joystick support AFTER checkForJoystick is called.
bool hasSupport();

//Starts a new thread listening for joystick events
//Updates the variables above with robot-ready parameters.
void listen();

}

#endif
