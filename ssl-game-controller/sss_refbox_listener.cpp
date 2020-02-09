#include "ssl-game-controller/sss_refbox_listener.h"
#include <iostream>

using namespace std;

/*! @brief The legacy Refbox packet
 * @details [RefBox legacy format](http://robocupssl.cpe.ku.ac.th/referee:legacy-protocol)
 * This is the formati of packet recieved by the RefBox. */
 struct Packet {
     char command;               //!<ASCII chatacter sent as the current comment
     unsigned char counter;      //!<Incremented when a new command is sent
     unsigned char goals_blue;   //!<Number of Blue goals
     unsigned char goals_yellow; //!<Number of Yellow goals
     unsigned short time_left;   //!<Remaining time (Seconds)?
 } *lastPacket;


SSLRefBoxListener::SSLRefBoxListener(YAML::Node comm_node)
{
    cout << "--REFBOX " << endl
         << "        REFBOX_ADDR    : " << comm_node["REFBOX_ADDR"] << endl
         << "        REFBOX_PORT    : " << comm_node["REFBOX_PORT"] << endl
         << "        REFBOX_ENABLED : " << comm_node["REFBOX_ENABLED"] << endl;
    _port       = comm_node["REFBOX_PORT"].as<int>();
    _net_address= comm_node["REFBOX_ADDR"].as<string>();
    cout << "--Refbox DONE" << endl;


    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress);
    socket->joinMulticastGroup(QHostAddress(QString(  _net_address.c_str()  ) ));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

SSLRefBoxListener::~SSLRefBoxListener()
{
    socket->close();
    delete socket;
}


void SSLRefBoxListener::readyRead(){
    QByteArray datagram;

    // using QUdpSocket::readDatagram (API since Qt 4)
    while (socket->hasPendingDatagrams()) {
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());

        lastPacket = (Packet*)datagram.constData();

        gamemodel->setGameState(lastPacket->command);
        gamemodel->setTimeLeft(lastPacket->time_left);
        gamemodel->setBlueGoals(lastPacket->goals_blue);
        gamemodel->setYellowGoals(lastPacket->goals_yellow);

    }


}






