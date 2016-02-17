#include <SDL2/SDL.h>
#include <thread>
#include "model/gamemodel.h"
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "gui/guiinterface.h"
#include "gui/joystick.h"

namespace joystick
{

bool configure(const std::string& name, int& jAxisMoveUp, int& jAxisMoveSide, int& jAxisRotate)
{
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

namespace joystick
{

//Thread to listen for joystick inputs
std::thread joystickThread;

//Is a *supported* joystick conencted? Used in hasSupport
bool hasSupportedJoystick = true;

//Calculated from Joystick movements
float LB, LF, RB, RF;
bool  Kick, Dribble;

void listener()
{
    int quit = 0;  //Quit?

    //Initialize SDL and Joystick support
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    while(!quit)
    {
        //Wait until we have a supported joystick
        while(SDL_NumJoysticks() == 0) {
            SDL_Delay(500);
            SDL_JoystickUpdate();
        }

        //Open joystick 0. Get name
        SDL_Joystick* joy = SDL_JoystickOpen(0);
        std::string name = SDL_JoystickName(joy);

        //Check if the joystick is supported. If it is, we go into a listening loop
        int jAxisMoveUp, jAxisMoveSide, jAxisRotate;
        if(configure(name, jAxisMoveUp, jAxisMoveSide, jAxisRotate))
        {
            //Let outside know we have a supported joystick connected
            hasSupportedJoystick = true;

            std::cout
                << '\n'
                << "Joystick Control Enabled\n"
                << name + '\n'
                << "********************************\n";

            //Variables to read joystick info
            float axes[8] = {0};               //Joystick axis readings buffer
            Uint8 buttons[50] = {0};           //Joystick button readings buffer
            Movement::FourWheelCalculator fwc; //Used to calculate velocites from joystick buffers

            //This is set to false when the joystick is removed
            bool joystickConnected = true;
            while(joystickConnected)
            {
                SDL_Delay(5);

                //Use SDL to tell us axis position and button presses
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    switch(event.type)
                    {
                    case SDL_JOYAXISMOTION:
                        axes[event.jaxis.axis] = 127 * ((float)event.jaxis.value / 32768);
                        break;
                    case SDL_JOYBUTTONDOWN:
                    case SDL_JOYBUTTONUP:
                        buttons[event.jbutton.button] = event.jbutton.state;
                        break;
                    case SDL_JOYDEVICEREMOVED:
                        std::cout << std::string("Joystick ") + SDL_JoystickName(joy) << " Removed" << std::endl;
                        joystickConnected = false;
                        hasSupportedJoystick = false;
                        SDL_JoystickClose(joy);
                        break;
                    case SDL_QUIT:
                        quit = 1;
                        break;
                    }
                }

                //The vels calculated are for the selected overridden robot.
                Robot* r = gameModel->findMyTeam(GuiInterface::getGuiInterface()->getSelOverBot());
                if(r != NULL)
                {
                    Point p = r->getPosition();
                    float o = r->getOrientation();
                    float tPos =   o +     -(M_PI/180)*axes[jAxisRotate];
                    float xPos = p.x + 3 *  axes[jAxisMoveSide];
                    float yPos = p.y + 3 * -axes[jAxisMoveUp];
                    auto v = fwc.calculateVels(r, xPos, yPos, tPos, Movement::Type::Default);
                    LB = v.LB; LF = v.LF;
                    RB = v.RB; RF = v.RF;
                    Kick = buttons[1];
                    Dribble = buttons[0];
                }
            }
        } else {
            hasSupportedJoystick = false;
            //When a non-support joystick is plugged in, keep giving a message every 5 seconds
            static clock_t time = 0;
            if((float)(clock() - time) / CLOCKS_PER_SEC > 5) {
                time = clock();
                std::cout
                    << '\n'
                    << "Joystick \"" + name + "\" is not supported.\n"
                    << "********************************\n";
            }
        }
    }

    SDL_Quit();
}

void listen()
{
    joystickThread = std::thread(listener);
}

bool hasSupport()
{
    return hasSupportedJoystick && (SDL_NumJoysticks() != 0);
}

}
