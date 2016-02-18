#include <SDL2/SDL.h>
#include <thread>
#include "model/gamemodel.h"
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "gui/guiinterface.h"
#include "gui/joystick.h"
#include "utilities/debug.h"

//Configuration section
namespace joystick
{

//A struct to hold axis movement configuration for each joystick
struct axis_configuration
{
    int jAxisMoveUp;    //What is the axis to move up?
    int jAxisMoveSide;  //What is the axis to move sideways?
    int jAxisRotate;    //What is the axis to rotate
};

//Stores configs for all connected joysticks
axis_configuration axisConfigs[10];

bool configure(const std::string& name, axis_configuration& config_out)
{
    int& jAxisMoveUp = config_out.jAxisMoveUp;
    int& jAxisMoveSide = config_out.jAxisMoveSide;
    int& jAxisRotate = config_out.jAxisRotate;

    if(name == "Saitek P990 Dual Analog Pad") {
        jAxisMoveUp = 1;
        jAxisMoveSide = 0;
        jAxisRotate = 3;
    } else if(name == "Logitech Logitech Dual Action") {
        jAxisMoveUp = 1;
        jAxisMoveSide = 0;
        jAxisRotate = 2;
    } else if(name == "Logitech Logitech Freedom 2.4") {
        jAxisMoveUp = 1;
        jAxisMoveSide = 0;
        jAxisRotate = 2;
    } else if(name == "Microsoft X-Box 360 pad") {
        jAxisMoveUp = 1;
        jAxisMoveSide = 0;
        jAxisRotate = 3;
    } else {
        return false;
    }
    return true;
}

}

//Command line mapping sections
namespace joystick
{


reading joystickReadings[10];

void map_joystick(const std::vector<std::string>& args)
{
    if(args.size() != 2) {
        std::cerr << "Usage: map_joy <joy-id> <rob-id>";
        return;
    }

    //Update the joystick at joy_id to point to rob_id
    int joy_id = std::atoi(args[0].c_str());
    int rob_id = std::atoi(args[1].c_str());
    joystickReadings[joy_id].id = rob_id;

    std::cout << "Joystick \"" << SDL_JoystickNameForIndex(joy_id) << "\" mapped to "
              << rob_id << std::endl;
}

void print_joymap(const std::vector<std::string>&)
{
    SDL_JoystickID joy = 0;
    for(; joy < 10; ++joy) {
        const reading& r = joystickReadings[joy];
        if(r.id != -1) {
            const char* name = SDL_JoystickNameForIndex(joy);
            std::cout << joy << ": \"" << name << "\" connected to robot " << r.id << std::endl;
        }
    }
}

}

//Main event listener section
namespace joystick
{

//Thread to listen for joystick inputs
std::thread joystickThread;

bool registerJoystick(SDL_JoystickID id, const std::string& name)
{
    /* Configure will return true if we have a recognized joystick,
     * and fill axisConfigs[id] with the proper joystick axis mapping
     * if it does. Otherwise, it returns false and does nothing. */
    if(configure(name, axisConfigs[id])) {
        std::cout
            << '\n' << "Joystick Control Enabled\n"
            << name + '\n'
            << "********************************\n";
        SDL_JoystickOpen(id);
        return true;
    } else {
        std::cout
            << '\n' << "Joystick \"" + name + "\" is not supported.\n"
            << "********************************\n";
        return false;
    }
}

Movement::FourWheelCalculator fwc;

float mult = 25;

Movement::fourWheelVels calculateRobotVelocity
    (int robotID, SDL_JoystickID joyID, float* axes)
{
    //Get the proper axis configurations
    axis_configuration conf = axisConfigs[joyID];

    //Find the robot, and calculate movement based on the joystick axes.
    Robot* r = gameModel->findMyTeam(robotID);
    if(r) {
        Point p = r->getRobotPosition();
        float o = r->getOrientation();
        float tPos =   o + 10* (M_PI/180)* axes[conf.jAxisRotate];
        float xPos = p.x +      mult * -axes[conf.jAxisMoveSide];
        float yPos = p.y +      mult *  axes[conf.jAxisMoveUp];
        auto velocity = fwc.calculateVels(r, xPos, yPos, tPos, Movement::Type::Default);
        return velocity;
    }

    //Should never get here anyway
    std::cout << "Robot not found " << robotID << std::endl;
    return Movement::fourWheelVels();
}

void listener()
{
    int quit = 0;  //Quit?

    //Initialize SDL and Joystick support
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    //Variables to read joystick info
    float axes[10][8] = {0};      //Joystick axis readings buffer
    Uint8 buttons[10][20] = {0};  //Joystick button readings buffer

    while(!quit)
    {
        SDL_Delay(5);

        //Use SDL to tell us axis position and button presses
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_JOYAXISMOTION:
                axes[event.jaxis.which][event.jaxis.axis] = 127 * ((float)event.jaxis.value / 32768);
                break;
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                buttons[event.jaxis.which][event.jbutton.button] = event.jbutton.state;
                break;
            case SDL_JOYDEVICEADDED: {
                SDL_JoystickID id = event.jdevice.which;
                registerJoystick(id, SDL_JoystickNameForIndex(id));
                break;
            }
            case SDL_QUIT:
                quit = 1;
                break;
            }
        }

        /* For each joystick reading, calculate the velocities for the robot it corrisponds to
         * and fill in the structure to be read by the GUI */
        SDL_JoystickID joy = 0;
        for(; joy != 10; ++joy)
        {
            reading& value = joystickReadings[joy];
            if(value.id != -1) {
                auto vels = calculateRobotVelocity(value.id, joy, axes[joy]);
                value.LB = vels.LB;
                value.LF = vels.LF;
                value.RB = vels.RB;
                value.RF = vels.RF;
                value.Kick = buttons[joy][0];
                value.Dribble = buttons[joy][1];
            }
        }
    }

    SDL_Quit();
}

void listen()
{
    debug::registerFunction("mapjoy", map_joystick);
    debug::registerFunction("p", print_joymap);
    debug::registerVariable("m", &mult);

    joystickThread = std::thread(listener);
}

bool hasSupport()
{
    return SDL_NumJoysticks() != 0;
}

}
