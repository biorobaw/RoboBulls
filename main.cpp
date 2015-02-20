#include <QApplication>
#include <stdio.h>
#include <csignal>
#include "communication/visioncomm.h"
#include "communication/robcomm.h"
#include "communication/refcomm.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "gui/guiinterface.h"
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
    static const int bad_signals[] = {SIGSEGV, SIGKILL, SIGHUP, SIGABRT, SIGTERM, SIGQUIT};
    for(int i : bad_signals)
        std::signal(i, exitStopRobot);
    std::atexit(exitStopRobot);
    std::set_terminate(exitStopRobot);
}



int main(int argc, char *argv[])
{
    StrategyController *sc = new StrategyController();
    GameModel * myGameModel = GameModel::getModel();
    sc->setGameModel(myGameModel);
    myGameModel->setStrategyController(sc);

    QApplication a(argc, argv);

    GuiInterface::getGuiInterface()->show();

    RefComm refCommunicator(myGameModel);
    VisionComm visionCommunicator(myGameModel);

    registerExitSignals();
    
    visionCommunicator.start();
    refCommunicator.start();
    return a.exec();
}
