#include <QApplication>
#include <stdio.h>
#include <csignal>
#include "communication/visioncomm.h"
#include "communication/robcomm.h"
#include "communication/refcomm.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "gui/guiinterface.h"
#include "utilities/debug.h"
#include "strategy/strategycontroller.h"

void exitStopRobot(int)
{
    for(Robot* rob : gameModel->getMyTeam()) {
        rob->setL(0);
        rob->setR(0);
    }
    RobComm::getRobComm()->sendVelsLarge(gameModel->getMyTeam());
    exit(1);
}

void exitStopRobot()
{
    exitStopRobot(-1);
}

void registerExitSignals()
{
    static const int bad_signals[] =
        {SIGSEGV, SIGKILL, SIGHUP, SIGABRT, SIGTERM, SIGQUIT};
    for(int i : bad_signals)
        std::signal(i, exitStopRobot);
    std::atexit(exitStopRobot);
    std::set_terminate(exitStopRobot);
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

    //Create the GUI and show it
    QApplication a(argc, argv);
    GuiInterface::getGuiInterface()->show();

    //Start Vision and Refcomm and run the application
    debug::listenStart();
    visionCommunicator.start();
    refCommunicator.start();
    return a.exec();
}
