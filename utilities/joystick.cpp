#ifndef ROBOBULLS_NOJOYSTICK

#include <SDL2/SDL.h>
#include <thread>
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "utilities/joystick.h"

namespace joystick
{

//Thread to listen for joystick inputs
std::thread joystickThread;

void listener(float* LB, float* LF, float* RB, float* RF, bool* kick)
{
    int quit = 0;
    *LB = *LF = *RB = *RF = *kick = 0;
    
    //We will quit if there are no joysticks plugged in
    SDL_Joystick* joy = SDL_JoystickOpen(0);
    if(joy == NULL) {
        fprintf(stderr, "No joysticks found. Terminating listen thread");
        quit = 1;
    }
    
    /********************************************************/
    
    float axes[8] = {0};
    Uint8 buttons[50] = {0};
    SDL_Event event;
    Movement::FourWheelCalculator fwc;
    
    while(!quit)
    {
        SDL_Delay(10);
        
        while (SDL_PollEvent(&event)) {
            switch(event.type)
            {
            case SDL_JOYAXISMOTION:
                axes[event.jaxis.axis] = 127 * ((float)event.jaxis.value / 32768);
                break;
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                buttons[event.jbutton.button] = event.jbutton.state;
                break;
            case SDL_QUIT:
                quit = 1;
                break;
            }
        }
        
        //This is for the Saitek P990. TODO: Make seperate functions for different controllers.
        *kick = buttons[1];
        float tPos = (M_PI/180)*axes[3];
        float xPos = 4*axes[0];
        float yPos = 4*axes[1];
        auto v = fwc.calculateVels(0, 0, 0, xPos, yPos, tPos);
        *LB = v.LB;
        *LF = v.LF;
        *RB = v.RB;
        *RF = v.RF;

        std::cout << *LB << *LF << std::endl;
    }

    SDL_JoystickClose(0);
    SDL_Quit();
}

void init()
{
    //Check out `https://github.com/Grumbel/sdl-jstest`
    //   would have never figured out this hint otherwise
    //Worth noting this is a different thread than listen function
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
}

void listen(float* LB, float* LF, float* RB, float* RF, bool* kick)
{
    joystickThread = std::thread(listener, LB, LF, RB, RF, kick);
}

bool checkForJoystick()
{
    return (SDL_NumJoysticks() != 0);
}

}

#endif
