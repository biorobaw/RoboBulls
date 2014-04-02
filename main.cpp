#include <QCoreApplication>
#include "communication/refcomm.h"
#include "Model/gamemodel.h"
#include "communication/robcomm.h"
#include "Robot/robot.h"
#include "include/messages_robocup_ssl_detection.pb.h"
#include "include/messages_robocup_ssl_geometry.pb.h"
#include "include/messages_robocup_ssl_wrapper.pb.h"
#include "include/robocup_ssl_client.h"

// RefComm Test
int main(int argc, char *argv[])
{
    //array for communication values 3 robots.
    char comm[13] = {'~', 0,0,0,0,8,500,500,0,0,0,0,0};

    StrategyController *sc = new StrategyController();
    //GameModel * myGameModel = new GameModel(sc);
    GameModel * myGameModel = GameModel::getModel();
    sc->setGameModel(myGameModel);
    myGameModel->setStrategyController(sc);

    QCoreApplication a(argc, argv);
    
    RefComm refCommunicator(myGameModel);

    robComm nxtbee;

    nxtbee.send(&comm[0], 13);

//    Robot myRobots[MAX_ROBOTS];

//    RoboCupSSLClient client;
//    client.open(true);
//    SSL_WrapperPacket packet;

    refCommunicator.run();

    return a.exec();


}
