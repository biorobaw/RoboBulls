#include "joystick.h"
#include <iostream>
using std::cout, std::endl, std::cerr;

#define DEAD_ZONE 0.1

struct JoyStick::JoyConfig {
    JoyConfig(std::string name, int slow, int kick, int dribble,
              int up, int side, int rotate)
        : name(name), buttonSlow(slow), buttonKick(kick), buttonDribble(dribble),
          axisUp(up), axisSide(side), axisRotate(rotate)
    {}
    std::string name  = "";
    int buttonSlow    = 0;
    int buttonKick    = 1;
    int buttonDribble = 2;
    int axisUp   = 0;    // What is the axis to move up?
    int axisSide = 1;    // What is the axis to move sideways?
    int axisRotate   = 2;
};

// list of supported joysticks:
JoyStick::JoyConfig JoyStick::supported_joysticks[] = {
    JoyConfig("Saitek P990 Dual Analog Pad"   , 7, 0, 1, 1, 3, 0),
    JoyConfig("Logitech Logitech Dual Action" , 7, 0, 1, 1, 2, 0),
    JoyConfig("Logitech Logitech Freedom 2.4" , 7, 0, 1, 1, 2, 0),
    JoyConfig("Microsoft X-Box 360 pad"       , 7, 0, 1, 1, 3, 0),
    JoyConfig("XInput Controller #2"          , 5, 0, 2, 1, 3, 0)
};


JoyStick* JoyStick::load_joystick(int joy_id)
{

    // check valid joy id
    if(joy_id < 0 || joy_id >= SDL_NumJoysticks()){
        cerr << "ERROR: joystick " << joy_id << " does not exist, max is "
             << SDL_NumJoysticks()-1 << endl;
        return nullptr;
    }

    // check if joystick is already in use
    auto sdl_joy = SDL_JoystickFromInstanceID(joy_id);
    if(sdl_joy && SDL_JoystickGetAttached(sdl_joy)){
        cerr << "ERROR: joystick " << joy_id << " has already been mapped"<<endl;
        return nullptr;
    }

    // find the configuration for the selected joystick
    std::string name = SDL_JoystickNameForIndex(joy_id);
    JoyConfig* config = nullptr;
    for(auto& j : supported_joysticks)
        if(j.name == name){
            config = &j;
            break;
        }

    // config not found print error
    if(!config){
        cerr << "ERROR: Joy type '"<< name << "' is not supported" <<endl;
        return nullptr;
    }

    // create joystick and add it to the set of open joysticks
    JoyStick* aux = new JoyStick(joy_id, config);
    return aux;

}



JoyStick::JoyStick(int joy_id, JoyConfig* _config)
  : joy_id(joy_id), joystick(SDL_JoystickOpen(joy_id)), config(_config)

{
}

JoyStick::~JoyStick(){
    if(SDL_JoystickGetAttached(joystick))
        SDL_JoystickClose(joystick);
}

std::string JoyStick::getJoyName(){
    return SDL_JoystickName(joystick);
}

int JoyStick::getJoyId(){
    return joy_id;
}


float JoyStick::getAxisUp(){
    return -removeDeadZone(SDL_JoystickGetAxis(joystick,config->axisUp)/32768.0);
}
float JoyStick::getAxisSide(){
    return removeDeadZone(SDL_JoystickGetAxis(joystick,config->axisSide)/32768.0);
}
float JoyStick::getAxisRotate(){
    return removeDeadZone(SDL_JoystickGetAxis(joystick,config->axisRotate)/32768.0);
}
bool JoyStick::getSlowMode(){
    return SDL_JoystickGetButton(joystick,config->buttonSlow);
}
bool JoyStick::getKick(){
    return SDL_JoystickGetButton(joystick,config->buttonKick);
}
bool JoyStick::getDribble(){
    return SDL_JoystickGetButton(joystick,config->buttonDribble);
}

float JoyStick::removeDeadZone(float value){
    if(value < -0.1) value +=0.1;
    else if(value > 0.1) value -= 0.1;
    else value = 0;
    return value/0.9;
}









