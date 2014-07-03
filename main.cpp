#include <QCoreApplication>
#include <stdio.h>
#include <csignal>
#include "communication/refcomm.h"
#include "communication/visioncomm.h"
#include "communication/robcomm.h"
#include "communication/simrobcomm.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "include/messages_robocup_ssl_detection.pb.h"
#include "include/messages_robocup_ssl_geometry.pb.h"
#include "include/messages_robocup_ssl_wrapper.pb.h"
#include "include/robocup_ssl_client.h"
#include "behavior/behavior.h"

using namespace std;

void exitStopRobot(int param)
{
    GameModel* gm = GameModel::getModel();
    RobComm* nxt  = RobComm::getRobComm();

    for(Robot* rob : gm->getMyTeam()) {
        nxt->sendVels(0, 0, rob->getID());
    }
	
	exit(1);
}

// RefComm Test
int main(int argc, char *argv[])
{
    StrategyController *sc = new StrategyController();
    GameModel * myGameModel = GameModel::getModel();
    sc->setGameModel(myGameModel);
    myGameModel->setStrategyController(sc);

    QCoreApplication a(argc, argv);

    RefComm refCommunicator(myGameModel);
    VisionComm visionCommunicator(myGameModel);


	std::signal(SIGSEGV, exitStopRobot);
	std::signal(SIGABRT, exitStopRobot);
	std::signal(SIGTERM, exitStopRobot);
    std::signal(SIGHUP, exitStopRobot);
	
	
    visionCommunicator.start();
    refCommunicator.run();


    return a.exec();
}
