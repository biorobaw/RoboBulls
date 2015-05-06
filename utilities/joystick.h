#ifndef ROBOBULLS_NOJOYSTICK
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

//Initializes jyostick support. *Call this before any other calls*
void init();

//Returns true if there is a joystick connected to the system
bool checkForJoystick();

//Starts a new thread listening for joystick events
//Passes out direct wheel velocitis from joystick and a bool kick.
void listen(float* LB, float* LF, float* RB, float* RF, bool* kick);

}

#endif
#endif
