#ifndef REFCOMM_H
#define REFCOMM_H

#include <string>
#include <QtCore>
#include "include/netraw.h"
#include "model/gamemodel.h"

using namespace std;

class RefComm : public QThread
{
public:
    RefComm(GameModel *gm,
            int port = 10001,
            string net_ref_address="224.5.23.1",
            string net_ref_interface="");

    ~RefComm();


    //communicating with reciver
    bool open(bool blocking=false);
    void close();
    bool receive();

    //void RefThread();
    void run();

protected:
    static const int MaxDataGramSize = 7;
    char * in_buffer;
    Net::UDP mc; // multicast client
    int _port;
    string _net_address;
    string _net_interface;
    GameModel *gamemodel;
};

#endif // REFCOMM_H



