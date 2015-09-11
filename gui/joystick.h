#ifndef JOYSTICK_H
#define JOYSTICK_H

/*! @brief Joystick control support
 * @details *This is an internal file; it should not be used directly except to add joysticks.*
 *
 * Uses SDL to interface with a single-connected joystick to control
 * a robot. *listen* recieves any joystick events and updates the variables, which
 * the GUI MainWindow reads to control a robot if it is selected.
 *
 * **Currently supported joysticks**
 * - Saitek P990 Dual Analog Pad
 * - Logitech Dual Action
 * - Logitech Freedom 2.4
 *
 * Any one of these joysticks can be added and removed during runtime. If no joystick is connected,
 * the arrow keys can be used to control overridden robots.
 *
 * **Adding New Joysticks**<br><br>
 * Like an FPS game, robots are typically moved with the *left* joystick, and rotated
 * with the *right* joystick. Kick is usually in the "X" spot for a PlayStation controller.
 *
 * To add a new joystick, modify *configure* in the .cpp with a new if-else case on the hardware name,
 * and set *jAxisMoveUp*, *jAxisMoveSide*, and *jAxisRotate* to the up-down axis number of
 * the left joystick, the left-right axis of the left joystick, and the left-right axis of the
 * right joystick respectively. Run [sdl-jtest](https://github.com/Grumbel/sdl-jstest) to find out
 * this information.
 */

namespace joystick
{

//! @brief Information from joysticks to be sent to the overridden robot (do not use directly)
//! @{
extern float LB, LF, RB, RF;
extern bool  Kick, Dribble;
//! @}

//! @brief Starts a new thread listening for joystick events
//! @details Updates the variables above with robot-ready parameters.
void listen();

//! @brief Query joystick support
//! @details This function must be called after listen()
//! @return True if a supported joystick is connected to the system
bool hasSupport();

}

#endif
