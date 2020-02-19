#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <SDL2/SDL.h>


#include "utilities/debug.h"
#include "model/game_state.h"
#include "movement/go_to_pose.h"
#include "gui/guiinterface.h"

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
 * - Microsoft X-Box 360 pad
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

namespace JoyStick
{

const int MAX_JOYSTICKS = 10;

/* Constants defining how joystick inputs translate to robot motion commands */
const double MOTION_VEC_MULT = 25;    // Multipler of motion vector passed to movement interface
const double ROT_SPD_MULT = 5;        // Multiplier between analog stick input and robot rotation speed
const double SLOW_MULT = 0.2;         // Multiplier for slow-mode


/*! Starts a new thread listening for joystick events */
void listen();  // Runs listner on new thread
void listener();

/*! Joystick setup */
struct axis_configuration
{
    int jAxisMoveUp;    //What is the axis to move up?
    int jAxisMoveSide;  //What is the axis to move sideways?
    int jAxisRotate;    //What is the axis to rotate?
};

void map_joystick(const std::vector<std::string>& args);
bool configure(const std::string& name, axis_configuration& config_out);
bool hasSupport();

/*! Sets velocities on robot objects */
void setCommands();

}   // namespace JoyStick

#endif
