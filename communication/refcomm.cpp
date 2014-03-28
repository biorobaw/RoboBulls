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
    in_buffer=new char[20];
    gamemodel = gm;

}

RefComm::~RefComm(void)
{
    delete[] in_buffer;
    //CloseHandle(hThread); //stop thread
}

void RefComm::close() {
    mc.close();
    cout<<"close"<< endl;
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

bool RefComm::receive(){
    Net::Address src;
    int r=0;

    r = mc.recv(in_buffer,MaxDataGramSize,src);

    if (r>0) {
        fflush(stdout);

        //decode packet:
//        cout<<in_buffer[0]<<endl;
        gamemodel->setGameState(in_buffer[0]);
//        this->command = in_buffer [0];
//        this->command_counter = (unsigned char) in_buffer[1];
//        this->goals_blue = (unsigned char) in_buffer [2];
//        this->goals_yellow = (unsigned char) in_buffer [3];
//        this->time_remaining = ((unsigned char)in_buffer[4] *256 + (unsigned char)in_buffer[5]);
        //cout<<"recieved"<<endl;
        return true;

    }
    //else


    return false;
}

void RefComm::run()
{
    open();
    while(true){
        receive();
    }
}

