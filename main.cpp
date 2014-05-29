#include <QCoreApplication>
#include <stdio.h>
#include "communication/refcomm.h"
#include "communication/visioncomm.h"
#include "model/gamemodel.h"
#include "communication/robcomm.h"
#include "model/robot.h"
#include "include/messages_robocup_ssl_detection.pb.h"
#include "include/messages_robocup_ssl_geometry.pb.h"
#include "include/messages_robocup_ssl_wrapper.pb.h"
#include "include/robocup_ssl_client.h"
#include "behavior/behavior.h"

void exitStopRobot(void)
{
    GameModel* mod = GameModel::getModel();
    auto team = mod->getMyTeam();

    robComm* nxt = robComm::getnxtbee();

    for(Robot* rob : team) {
        nxt->sendVels(0, 0, rob->getID());
    }
}

// RefComm Test
int main(int argc, char *argv[])
{
    StrategyController *sc = new StrategyController();
    GameModel * myGameModel = GameModel::getModel();
    sc->setGameModel(myGameModel);
    // myGameModel->play();
    myGameModel->setStrategyController(sc);

    QCoreApplication a(argc, argv);

    RefComm refCommunicator(myGameModel);
    VisionComm visionCommunicator(myGameModel);

    // Robot myRobots[MAX_ROBOTS];
    // RoboCupSSLClient client;
    // client.open(true);
    // SSL_WrapperPacket packet;

    atexit(exitStopRobot);

    visionCommunicator.start();
    refCommunicator.run();

    return a.exec();
}
