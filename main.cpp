#include <QApplication>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include "include/config/simulated.h"
#include "include/config/team.h"
#include "include/config/communication.h"
#include "communication/visioncomm.h"
#include "communication/robcomm.h"
#include "communication/refcomm.h"
#include "model/gamemodel.h"
#include "gui/guiinterface.h"
#include "utilities/debug.h"
#include "strategy/strategycontroller.h"

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
 * See the Config Module in Modules. The two most important are:
 * - *include/config/simulated.h*--Simulated or Field play-- The SIMULATED macrco
 *   selects to run the project on the field (ENG lab) or grSim simulator.
 * - *include/config/team.h*--Team Selection--The TEAM macro selects TEAM_BLUE or TEAM_YELLOW.
 *
 * @section start Getting Started--Running The Project
 * Here we are going to compile and run the project on the simulator and perform a simple
 * Behavior.<br><br>
 * 1) Open the RoboBulls.pro project in Qt Creator and compile.
 *    After running the installation script, there should be no compilation issues
 *    (Tested on Ubuntu 14.04 only)<br>
 * 2) Go to **communication/simrobcomm.cpp** and change <b>_addr</b> in the SimRobComm
 *    constructor to the IP of the computer running the simulator (127.0.0.1 for local).<br>
 * 3) Go to **strategy/strategycontroller.cpp** and change USE_TEST_STRATEGY to 1<br>
 * 4) Go to **strategy/teststrategy.cpp** and insert
 *    **gameModel->findMyTeam(0)->assignBeh<GoToBeh>();** under **TestStrategy::assignBeh**<br>
 * 5) Open the grSim simulator (grSim/bin/grsim) from the script's install path
 *    on your computer or any other computer<br>
 * 6) Making sure SIMULATED in include/config/simulated is 1, Run the program.
 *
 * If these steps are completed correctly, Robot 0 on TEAM should move to the center
 * of the field.
 */

//! @brief Sets robots velocities to zero and exits the program
void exitStopRobot(int)
{
    for(Robot* rob : gameModel->getMyTeam()) {
        rob->setL(0);
        rob->setR(0);
    }
    RobComm::getRobComm()->sendVelsLarge(gameModel->getMyTeam());
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
    for(int bad_signal : {SIGSEGV, SIGKILL, SIGHUP, SIGABRT, SIGTERM, SIGQUIT})
        std::signal(bad_signal, exitStopRobot);
    std::atexit(exitStopRobot);
    std::set_terminate(exitStopRobot);
}

//! @brief Print a message about build time and useful config information
void printBuildInfo()
{
    std::cout
        << "RoboBulls 2 Build " << __DATE__ << " " << __TIME__ << std::endl
        << "       Team: " << ((TEAM == TEAM_BLUE) ? "Blue" : "Yellow") << std::endl
        << "  Simulated: " << SIMULATED << std::endl
        << "  Simulator: " << SIMULATOR_ADDRESS << ":" << SIMULATOR_PORT << std::endl
        << "     Vision: " << VISION_ADDRESS    << ":" << VISION_PORT    << std::endl
        << "     RefBox: " << REFBOX_LISTEN_ENABLED << std::endl;
}

int main(int argc, char *argv[])
{
    //Initialize GameModel, StrategyController, Vision, and Ref
    GameModel* gm = GameModel::getModel();
       RefComm refCommunicator(gm);
    VisionComm visionCommunicator(gm);
    StrategyController sc(gm);
    gm->setStrategyController(&sc);
    registerExitSignals();

    //Build message
    printBuildInfo();

    //Create the GUI and show it
    QApplication a(argc, argv);
    GuiInterface::getGuiInterface()->show();

    //Start Vision and Refcomm and run the application
    debug::listenStart();
    visionCommunicator.start();
    refCommunicator.start();
    return a.exec();
}
