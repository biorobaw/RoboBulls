#include <QApplication>
#include <QDir>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include "ssl-vision/ssl_vision_listener.h"
#include "robot/robcomm.h"
#include "ssl-game-controller/sss_refbox_listener.h"
#include "model/game_state.h"
#include "gui/guiinterface.h"
#include "utilities/debug.h"
#include "strategy/strategycontroller.h"
#include "strategy/controllers/joystick/scontroller_joystick.h"
#include "yaml-cpp/yaml.h"
#include <string>
#include "model/field.h"

#include "parameters/motion_parameters.h"

/*! @mainpage Welcome to the RoboBulls 2 Documentation.
 *
 * @section install Installation
 * The required libraries and tools to run the game can be installed by
 * running <b>documents/scripts/install_robobulls.sh</b> found with the Git download.
 * By default, it installs in `$HOME`, but check out the script to change it.
 * if "robobulls2" is not the installation path, it will also be downloaded.
 *
 * The Installation Script will download and compile the following.
 * - All required libraries to compile and run the code and tools
 * - The [grSim Simulator](https://github.com/mani-monaj/grSim),
 *   the program used to simulate a game.
 * - The [SSL Referee Box](http://robocupssl.cpe.ku.ac.th/referee:start),
 *   the standard control platform for the game.
 * - The [RoboBulls2 Project](https://github.com/mllofriu/robobulls2),
 *   the project itself.
 *
 * @section config Configuration
 * The project is controlled by a number of configuration files.
 * See the Config Module in Modules. The three most important are:
 * - *include/config/simulated.h*--Simulated or Field play-- The SIMULATED macrco
 *   selects to run the project on the field (ENG lab) or grSim simulator.
 * - *include/config/team.h*--Team Selection--The TEAM macro selects TEAM_BLUE or TEAM_YELLOW.
 * - *include/config/communication.h*--Network config--Sets the addresses and ports of the
 *    communication modules DEPRECATED
 * UPDATE: some config files are no longer header files but yaml files to avoid recompilation,
 * files are now located in "config/*.yaml". Each file should be self explanatory. When the RoboBulls
 * software starts, it will look for the configuration files found in folder config.
 *
 * @section start Getting Started--Running The Project
 * Here we are going to compile and run the project on the simulator and perform a simple
 * Behavior.<br><br>
 * 1) Open the RoboBulls.pro project in Qt Creator and compile.
 *    After running the installation script, there should be no compilation issues
 *    (Tested on Ubuntu 14.04 only)<br>
 * 2) Open the grSim simulator (grSim/bin/grsim) from the script's install path
 *    on your computer or any other computer<br>
 * 3) Go to **include/config/communication.h** (DEPRECATED, file replaced by "config/comm.yaml")
 *    and change <b>SIMULATOR_ADDRESS</b> to the IP
 *    (in quotes) of the computer running grSim (SIMULATOR_ADDRESS_LOCAL for local).<br>
 * 4) Change <b>REFBOX_LISTEN_ENABLED</b> to 0--This will enable of the TestStrategy below.<br>
 * 5) Change <b>VISION_ADDRESS</b> under the #if SIMULATED to the "Vision multicast address" shown on the
 *    left-hand side of the simulator<br>
 * 6) Go to **strategy/teststrategy.cpp** and insert
 *    **gameModel->findMyTeam(0)->assignBeh<GoToBeh>();** under **TestStrategy::assignBeh**<br>
 * 7) Making sure <b>SIMULATED</b> in include/config/simulated is 1, Run the program.
 *
 * If these steps are completed correctly, Robot 0 on TEAM should move to the center
 * of the field.
 *
 * @example example_gamemodel.cpp
 * @example example_genericmovement.cpp
 * @example example_kicktopointomni.cpp
 */

//! @brief Sets robots velocities to zero and exits the program
void exitStopRobot(int)
{
    //TODO: make a clean exit
    std::cout << "This function sould be called only once, not thrice!" <<std::endl;
    exit(1);
}

//! @brief Signature required by atexit
void exitStopRobot()
{
    exitStopRobot(-1);
}

//! @brief Registers exit signals to stop the robots on a crash or program close
void registerExitSignals()
{
//    for(int bad_signal : {SIGSEGV, SIGKILL, SIGHUP, SIGABRT, SIGTERM, SIGQUIT})
    for(int bad_signal : {SIGSEGV,  SIGABRT, SIGTERM }) // some signals removed so that it will also work in windows
        std::signal(bad_signal, exitStopRobot);
    std::atexit(exitStopRobot);
    std::set_terminate(exitStopRobot);
}

class MyApplication : public QApplication {
public:

    MyApplication(int argc, char* argv[]) : QApplication(argc,argv){}

    bool notify(QObject* receiver, QEvent* event) override {
        bool done = true;
        try {
            done = QApplication::notify(receiver, event);
        } catch (const std::exception& ex) {
            std::cout << " what? " << ex.what() << std::endl;
        } catch (const std::string& ex) {
            std::cout << " what? " << ex << std::endl;
        } catch (...) {
            std::cout << " what? " << "unnamed exception" << std::endl;
        }
        return done;
    }


};

#undef main // this is to prevent multiple definitions of main from external modules like SDL
int main(int argc, char *argv[])
{

    std::cout << "WORKING!!!";
    std::string folder = argc > 1 ? argv[1] : "./config";

    std::cout << QDir::currentPath().toStdString() << std::endl;

    std::cout << "-- COMMAND ARGS (" << argc << ")" <<  std::endl;
    for(int i=0; i < argc; i++)
        std::cout << "        arg (" << i << ") : " << argv[i] << std::endl;
    // Load config files:
    YAML::Node comm_node = YAML::LoadFile(folder + "/comm.yaml");
    YAML::Node team_node = YAML::LoadFile(folder + "/team.yaml");
    YAML::Node field_node = YAML::LoadFile(folder + "/field.yaml");
    YAML::Node motion_node = YAML::LoadFile(folder + "/motion.yaml");


    MyApplication a(argc, argv);
    SControllerJoystick::init_module(); // init joystick listener module

    // set all parameters:
    Field::load(field_node);
    load_motion_parameters(motion_node);

    // load teams:
    RobotTeam::load_teams(team_node);



    //Initialize GameModel, StrategyController, Vision, and Ref

    SSLRefBoxListener refCommunicator( comm_node);
    // TODO: vision communicator should not know anything about team sides (deprecated notion of "own team")
    SSLVisionListener visionCommunicator(comm_node);


    registerExitSignals();

    //Create the GUI and show it
    GuiInterface::getGuiInterface()->show();

    // Start the debugger thread
    debug::listenStart();



    // start threads and wait for program to exit
    visionCommunicator.start();
    refCommunicator.start();
    int result = a.exec();


    // stop threads
    SControllerJoystick::stop_module();
    refCommunicator.stop();
    refCommunicator.wait();

    visionCommunicator.stop();
    visionCommunicator.wait();

    for(int i=0;i<2;i++)
        RobotTeam::getTeam(i)->closeCommunication();

    // wait for threads to close:
    std::cout<< "result: " << result << std::endl;

    return result;


}
