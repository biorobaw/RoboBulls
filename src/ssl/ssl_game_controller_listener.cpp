// WARNING: We need to import all ssl_ files, in some systems, the order seems to be important
#include "ssl_game_event.pb.h"
#include "ssl_game_controller_common.pb.h"
#include "ssl_game_event_2019.pb.h"
#include "ssl_referee.pb.h"

#include "ssl_game_controller_listener.h"
#include "utilities/my_yaml.h"
#include "model/team/team.h"
#include "model/game_state.h"




SSLGameControllerListener* SSLGameControllerListener::instance = nullptr;
QThread* SSLGameControllerListener::thread = nullptr;

SSLGameControllerListener::SSLGameControllerListener(YAML::Node* comm_node, RobotTeam* teams[]) :
    QObject(nullptr)
{
    qInfo() << "--REFBOX ";
    qInfo() << "        REFBOX_ADDR    :" << (*comm_node)["REFBOX_ADDR"].Scalar().c_str();
    qInfo() << "        REFBOX_PORT    :" << (*comm_node)["REFBOX_PORT"].Scalar().c_str();

    net_address= (*comm_node)["REFBOX_ADDR"].as<string>().c_str();
    net_port       = (*comm_node)["REFBOX_PORT"].as<int>();

    command = Referee_Command_HALT;
    command_previous = Referee_Command_HALT;

    restart_socket();

    thread = new QThread;
    this->moveToThread(thread);
    connect(thread, &QThread::finished, this, &QObject::deleteLater);

    instance = this;
    qInfo() << "--Refbox DONE - thread: " << thread;

    for(int i=0; i<2; i++){
        auto game_state = teams[i]->getGameState();
        connect(this, &SSLGameControllerListener::goalsChanged,game_state, &GameState::goalsChanged);
        connect(this, &SSLGameControllerListener::refereeCommandChanged,game_state, &GameState::refereeCommandChanged);

    }

}


void SSLGameControllerListener::restart_socket(){
    if(socket->isOpen()) socket->close();

    if(!socket->bind(QHostAddress::AnyIPv4, net_port, QUdpSocket::ShareAddress)){
        qFatal("ERROR: could not bind refbox port %d", net_port);
    }

    if(!socket->joinMulticastGroup(QHostAddress(net_address ))){
        qFatal("ERROR: ssl-refbox could not join multicast group %s", net_address.toUtf8().constData());
    }

    connect(socket,&QUdpSocket::readyRead, this , &SSLGameControllerListener::process_package);

}

SSLGameControllerListener::~SSLGameControllerListener(){
    thread->exit();
    thread->wait();
    instance = nullptr;
}

void SSLGameControllerListener::copyState(GameState* game_state){
    if(instance == nullptr) return;
    instance->mutex.lock();
        game_state->remaining_time = instance->time_left;
    instance->mutex.unlock();

}

SSLGameControllerListener* SSLGameControllerListener::get(){
    return instance;
}


void SSLGameControllerListener::process_package(){
    QByteArray datagram;

    while(socket->hasPendingDatagrams()){

        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());

        Referee referee;
        if(!referee.ParseFromArray(datagram.data(),datagram.size())) continue;



        if(referee.has_command() && referee.command() != command ) {
            command_previous = command;
            command = referee.command();
            emit this->refereeCommandChanged(command, command_previous);
        }


        bool emit_goals_changed = false;
        if(referee.has_blue() && referee.blue().has_score()){
            emit_goals_changed |= goals[ROBOT_TEAM_BLUE] != referee.blue().score();
            goals[ROBOT_TEAM_BLUE] = referee.blue().score();
        }
        if(referee.has_yellow() && referee.yellow().has_score()){
            emit_goals_changed |= goals[ROBOT_TEAM_BLUE] != referee.blue().score();
            goals[ROBOT_TEAM_YELLOW] = referee.yellow().score();
        }
        if(emit_goals_changed) emit this->goalsChanged(goals[ROBOT_TEAM_BLUE],goals[ROBOT_TEAM_YELLOW]);




        mutex.lock();
            // time left is polled so it is withing mutexes
            if(referee.has_stage_time_left()) time_left = referee.stage_time_left();

        mutex.unlock();


    }

}

void run(){

}






