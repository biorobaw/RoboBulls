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

void exitStopRobot(int param)
{
    UNUSED_PARAM(param);
    for(Robot* rob : gameModel->getMyTeam()) {
        rob->setL(0);
        rob->setR(0);
    }
    RobComm::getRobComm()->sendVelsLarge(gameModel->getMyTeam());
    exit(1);
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

    std::signal(SIGSEGV, exitStopRobot);
    std::signal(SIGABRT, exitStopRobot);
    std::signal(SIGTERM, exitStopRobot);
    std::signal(SIGHUP, exitStopRobot);
    
    visionCommunicator.start();
    refCommunicator.start();

    return a.exec();
}
