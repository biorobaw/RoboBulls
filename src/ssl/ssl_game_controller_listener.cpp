// WARNING: We need to import all ssl_ files, in some systems, the order seems to be important
#include "ssl_game_event.pb.h"
#include "ssl_game_controller_common.pb.h"
#include "ssl_game_event_2019.pb.h"
#include "ssl_referee.pb.h"

#include "ssl_game_controller_listener.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include "model/game_state.h"



using namespace std;

SSLGameControllerListener* SSLGameControllerListener::instance = nullptr;

SSLGameControllerListener::SSLGameControllerListener(YAML::Node* comm_node) :
    QObject(nullptr)
{
    qInfo() << "--REFBOX ";
    qInfo() << "        REFBOX_ADDR    : " << (*comm_node)["REFBOX_ADDR"].Scalar().c_str();
    qInfo() << "        REFBOX_PORT    : " << (*comm_node)["REFBOX_PORT"].Scalar().c_str();

    command = Referee_Command_HALT;
    command_previous = Referee_Command_HALT;
    net_address= (*comm_node)["REFBOX_ADDR"].as<string>().c_str();
    port       = (*comm_node)["REFBOX_PORT"].as<int>();

    restart_socket();

    instance = this;
    qInfo() << "--Refbox DONE";
}


void SSLGameControllerListener::restart_socket(){
    if(socket->isOpen()) socket->close();

    if(!socket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress)){
        qFatal("ERROR: could not bind refbox port %d", port);
    }

    if(!socket->joinMulticastGroup(QHostAddress(net_address ))){
        qFatal("ERROR: ssl-refbox could not join multicast group %s", net_address.toUtf8().constData());
    }

    connect(socket,&QUdpSocket::readyRead, this , &SSLGameControllerListener::process_package);

}


void SSLGameControllerListener::copyState(GameState* game_state){
    if(instance == nullptr) return;
    instance->mutex.lock();
        game_state->referee_command_changed = game_state->referee_command != instance->command;
        game_state->referee_command = instance->command;
        game_state->referee_command_previous = instance->command_previous;
        game_state->remaining_time = instance->time_left;
        game_state->goals[0] = instance->goals[0];
        game_state->goals[1] = instance->goals[1];
    instance->mutex.unlock();

}


void SSLGameControllerListener::process_package(){
    QByteArray datagram;

    while(socket->hasPendingDatagrams()){

        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());

        Referee referee;
        if(!referee.ParseFromArray(datagram.data(),datagram.size())) continue;


//        cout << "Refbox: ";
//        if(referee.has_command()) cout << "C: " << referee.command() << " ";
//        if(referee.has_stage_time_left()) cout << "T: " << referee.stage_time_left() << " ";
//        if(referee.has_blue() &&
//           referee.blue().has_score()) cout << "B: " << referee.blue().score() << " ";
//        if(referee.has_yellow() &&
//           referee.yellow().has_score()) cout << "Y: " << referee.yellow().score() << " ";
//        cout << endl;

        mutex.lock();
            if(referee.has_command() && referee.command() != command ) {
                command_previous = command;
                command = referee.command();
            }
            if(referee.has_stage_time_left()) time_left = referee.stage_time_left();
            if(referee.has_blue() && referee.blue().has_score())
                goals[ROBOT_TEAM_BLUE] = referee.blue().score();
            if(referee.has_yellow() && referee.yellow().has_score())
                goals[ROBOT_TEAM_YELLOW] = referee.yellow().score();
        mutex.unlock();


    }

}

void run(){

}






