#include "gui/joystick.h"

//Configuration section
namespace JoyStick
{

/*! Mapping between joysticks and robot ids */
int robots[MAX_JOYSTICKS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

/*! Variables to read joystick info */
float axes[MAX_JOYSTICKS][8] = {0};      // Joystick axis readings buffer
Uint8 buttons[MAX_JOYSTICKS][20] = {0};  // Joystick button readings buffer

std::thread joystickThread;
axis_configuration axisConfigs[MAX_JOYSTICKS]; //Stores configs for all connected joysticks

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

// Command line mapping function. Creates entry in robots array.
void map_joystick(const std::vector<std::string>& args)
{
    if(args.size() != 2) {
        std::cerr << "Usage: mapjoy <joy-id> <rob-id>";
        return;
    }

    //Update the joystick at joy_id to point to rob_id
    int joy_id = std::atoi(args[0].c_str());
    int rob_id = std::atoi(args[1].c_str());
    robots[joy_id] = rob_id;

    std::cout << "Joystick \"" << SDL_JoystickNameForIndex(joy_id) << "\" mapped to "
              << rob_id << std::endl;
}

void print_joymap(const std::vector<std::string>&)
{
    SDL_JoystickID joy = 0;
    for(; joy < 10; ++joy)
    {
        int r_id = robots[joy];
        if(r_id != -1)
        {
            const char* name = SDL_JoystickNameForIndex(joy);
            std::cout << joy << ": \"" << name << "\" connected to robot " << r_id << std::endl;
        }
    }
}


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

void setCommands()
{
    // Set speed based on joystick axes
    for(int joy_id = 0; joy_id < MAX_JOYSTICKS; ++joy_id)
    {
        // Get the proper axis configurations
        axis_configuration conf = axisConfigs[joy_id];

        // If a robot has been mapped to this joystick
        if(robots[joy_id] != -1)
        {
            // Retrieve robot pointer
            Robot* r = gameModel->findMyTeam(robots[joy_id]);

            // If robot exists
            if(r)
            {
                // Calculate velocities to set
                Point p = r->getPosition();
                double o = r->getOrientation();
                bool slow_mode = buttons[joy_id][7];
                double trans_mult = slow_mode? MOTION_VEC_MULT*SLOW_MULT : MOTION_VEC_MULT;
                double oPos =   o +  ROT_SPD_MULT * (M_PI/180) * axes[joy_id][conf.jAxisRotate];
                double xPos = p.x -  trans_mult * axes[joy_id][conf.jAxisMoveSide];
                double yPos = p.y +  trans_mult * axes[joy_id][conf.jAxisMoveUp];

                // Set velocities on robot object through movement interface
                Move::GoToPose go_to_pose;
                go_to_pose.updateGoal(Point(xPos, yPos), oPos, false, false);
                go_to_pose.perform(r, Move::MoveType::Default);

                // Set button inputs
                r->setKick(buttons[joy_id][0]);
                r->setDribble(buttons[joy_id][1]);

                // GuiInterface::getGuiInterface()->drawPath(p, Point(xPos, yPos));
            }
        }
    }
}

void listener()
{
    int quit = 0;  //Quit?

    //Initialize SDL and Joystick support
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    //Hardcoding initial joystick mapping (comment out if not using joysticks override)
//#if TEAM == TEAM_YELLOW
//    map_joystick({"0", "2"});
//    map_joystick({"1", "3"});
//    map_joystick({"2", "4"});
//#else
//    map_joystick({"0", "1"});
//    map_joystick({"1", "5"});
//    map_joystick({"2", "6"});
//#endif

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
    }


    SDL_Quit();
}

void listen()
{
    debug::registerFunction("mapjoy", map_joystick);
    debug::registerFunction("p", print_joymap);
    joystickThread = std::thread(listener);
}

bool hasSupport()
{
    return SDL_NumJoysticks() != 0;
}

}
