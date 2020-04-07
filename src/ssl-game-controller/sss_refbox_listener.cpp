// WARNING: We need to import all ssl_ files, in some systems, the order seems to be important
#include "ssl_game_event.pb.h"
#include "ssl_game_controller_common.pb.h"
#include "ssl_game_event_2019.pb.h"
#include "ssl_referee.pb.h"

#include "ssl-game-controller/sss_refbox_listener.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include "model/game_state.h"
using namespace std;


SSLRefBoxListener::SSLRefBoxListener(YAML::Node* comm_node)
{
    cout << "--REFBOX " << endl
         << "        REFBOX_ADDR    : " << (*comm_node)["REFBOX_ADDR"] << endl
         << "        REFBOX_PORT    : " << (*comm_node)["REFBOX_PORT"] << endl;
    net_address= (*comm_node)["REFBOX_ADDR"].as<string>();
    port       = (*comm_node)["REFBOX_PORT"].as<int>();
    cout << "--Refbox DONE" << endl;


}

void SSLRefBoxListener::stop(){
    done = true;
}

void SSLRefBoxListener::run(){
    QByteArray datagram;
    done = false;

    cout << "Binding: " << net_address << " " << port <<endl;
    QUdpSocket socket;
    if(!socket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress)){
        cerr << "ERROR: could not bind refbox port " << port << endl;
        exit(-1);
    }
    if(!socket.joinMulticastGroup(QHostAddress(QString(  net_address.c_str()  ) ))){
        cerr << "ERROR: ssl-refbox could not join multicast group "<<  net_address <<endl;
        exit(-1);
    }

    // using QUdpSocket::readDatagram (API since Qt 4)
    while(!done){

        if(!socket.hasPendingDatagrams()) {
            msleep(5);
            continue;
        }

//        cout << "pending: " << socket.hasPendingDatagrams() << endl;
        datagram.resize(int(socket.pendingDatagramSize()));
        socket.readDatagram(datagram.data(), datagram.size());

        Referee referee;
//        datagram.data()
        if(referee.ParseFromArray(datagram.data(),datagram.size())){


            cout << "Refbox: ";
            if(referee.has_command()) cout << "C: " << referee.command() << " ";
            if(referee.has_stage_time_left()) cout << "T: " << referee.stage_time_left() << " ";
            if(referee.has_blue() &&
               referee.blue().has_score()) cout << "B: " << referee.blue().score() << " ";
            if(referee.has_yellow() &&
               referee.yellow().has_score()) cout << "Y: " << referee.yellow().score() << " ";
            cout << endl;

            if(referee.has_command()){
                GameState::setRefereeCommand(referee.command());
            }
            if(referee.has_stage_time_left())
                GameState::setTimeLeft( referee.stage_time_left() );
            if(referee.has_blue() && referee.blue().has_score())
                // seems to be in micro seconds
                GameState::setBlueGoals(referee.blue().score());
            if(referee.has_yellow() && referee.yellow().has_score())
                GameState::setYellowGoals(referee.yellow().score());

        }




    }

    socket.close();
}






