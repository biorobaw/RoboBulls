#include "src/ssl-game-controller/sss_refbox_listener.h"
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


}

void SSLRefBoxListener::stop(){
    done = true;
}

void SSLRefBoxListener::run(){
    QByteArray datagram;
    done = false;

    QUdpSocket socket(this);
    socket.bind(QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress);
    socket.joinMulticastGroup(QHostAddress(QString(  _net_address.c_str()  ) ));

    // using QUdpSocket::readDatagram (API since Qt 4)
    while(!done){
        if (socket.hasPendingDatagrams()) {
            datagram.resize(int(socket.pendingDatagramSize()));
            socket.readDatagram(datagram.data(), datagram.size());

            lastPacket = (Packet*)datagram.constData();

            GameState::setGameState(lastPacket->command);
            GameState::setTimeLeft(lastPacket->time_left);
            GameState::setBlueGoals(lastPacket->goals_blue);
            GameState::setYellowGoals(lastPacket->goals_yellow);

        } else {
            msleep(5);
        }
    }

    socket.close();
}






