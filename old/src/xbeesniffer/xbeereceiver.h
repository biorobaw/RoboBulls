#ifndef XBEERECEIVER_H
#define XBEERECEIVER_H

#include "serialib.h"

//! @brief Utility class to read bytes sent over an Xbee
class XbeeReceiver
{
public:
    XbeeReceiver();
    void receive();
private:
    serialib xbee;
};

#endif // XBEERECEIVER_H
