#include "communication/refcomm.h"
#include <iostream>

using namespace std;

RefComm::RefComm(GameState *gm, YAML::Node comm_node)
{
    cout << "--REFBOX " << endl
         << "        REFBOX_ADDR    : " << comm_node["REFBOX_ADDR"] << endl
         << "        REFBOX_PORT    : " << comm_node["REFBOX_PORT"] << endl
         << "        REFBOX_ENABLED : " << comm_node["REFBOX_ENABLED"] << endl;
    _port       = comm_node["REFBOX_PORT"].as<int>();
    _net_address= comm_node["REFBOX_ADDR"].as<string>();
    _net_interface="";
    gamemodel = gm;
    cout << "--Refbox DONE" << endl;
}

RefComm::~RefComm()
{
    close();
}

void RefComm::close() {
    std::cout << "Closing ref comm..." <<std::endl;
    mc.close();
    stop_listening = true;
    std::cout << "Closed ref comm" <<std::endl;
}

bool RefComm::open(bool blocking) {
//    close();
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
      fprintf(stderr,"RefBox: Unable to setup UDP multicast\n");
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
        gamemodel->setGameState(lastPacket.command);
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
    while(!stop_listening){
        receive();
    }
}



