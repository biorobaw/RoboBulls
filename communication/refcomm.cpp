#include "communication/refcomm.h"
#include <iostream>

using namespace std;

RefComm::RefComm(GameModel *gm,
                 int port,
                 string net_address,
                 string net_interface)
{
    _port=port;
    _net_address=net_address;
    _net_interface=net_interface;
    gamemodel = gm;
}

RefComm::~RefComm()
{
    close();
}

void RefComm::close() {
    mc.close();
//    cout<<"close"<< endl;
}

bool RefComm::open(bool blocking) {
    close();
    if(!mc.open(_port,true,true,blocking)) {
      fprintf(stderr,"Unable to open UDP network port: %d\n",_port);
      fflush(stderr);
      return(false);
    }

    Net::Address multiaddr,interface;
    multiaddr.setHost(_net_address.c_str(),_port);
    if(_net_interface.length() > 0){
      interface.setHost(_net_interface.c_str(),_port);
    }else{
      interface.setAny();
    }

    if(!mc.addMulticast(multiaddr,interface)) {
      fprintf(stderr,"Unable to setup UDP multicast\n");
      fflush(stderr);
      return(false);
    }

    return(true);
}

bool RefComm::receive()
{
    Net::Address src;
    int readBytes = 0;

    readBytes = mc.recv(&lastPacket, 6*sizeof(char), src);

    if (readBytes == 6) {
        char cmd = lastPacket.command;
        gamemodel->setGameState(cmd);
        gamemodel->setTimeLeft(lastPacket.time_left);
        gamemodel->setBlueGoals(lastPacket.goals_blue);
        gamemodel->setYellowGoals(lastPacket.goals_yellow);
        return true;
    }

    return false;
}

void RefComm::run()
{
    open();
    while(true){
        receive();
    }
}

