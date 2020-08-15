#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <string>
#include <SDL2/SDL.h>


#include "utilities/debug.h"
#include "gui/interface/gui_interface.h"


/*! @brief Joystick control support
 *
 * **Currently supported joysticks**
 * - Saitek P990 Dual Analog Pad
 * - Logitech Dual Action
 * - Logitech Freedom 2.4
 * - Microsoft X-Box 360 pad
 * - XInput Controller #2
 *
 * **Adding New Joysticks**<br><br>
 *
 * To add a new joystick, add an entry to the supported_joysticks array
 */


class JoyStick
{
private:
    struct JoyConfig;
    static JoyConfig supported_joysticks[];

public:
    // Loads a joystick object. It must be deleted when no longer needed
    static JoyStick* load_joystick(int joy_id);


    ~JoyStick();

    std::string getJoyName();
    int getJoyId();
    float getAxisUp();
    float getAxisSide();
    float getAxisRotate();
    bool  getSlowMode();
    bool  getKick();
    bool  getDribble();


private:
    JoyStick(int joy_id, JoyConfig* config);
    static float removeDeadZone(float);

    int joy_id;
    SDL_Joystick* joystick;
    JoyConfig* config;


};   // namespace JoyStick

#endif
