#ifndef XBEERECEIVER_H
#define XBEERECEIVER_H

#include "serialib.h"

class XbeeReceiver
{
public:
    XbeeReceiver();
    void receive();
private:
    serialib xbee;
};

#endif // XBEERECEIVER_H
