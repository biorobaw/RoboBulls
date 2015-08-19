#ifndef SIMROBCOMM_H
#define SIMROBCOMM_H
#include <QtNetwork/QUdpSocket>
#include "model/robot.h"
#include "robcomm.h"

//Percentage of stored robot velocity sent to simulator
const float mappingRatio = 0.333;

class SimRobComm : public RobComm
{
public:
    SimRobComm();
    virtual void sendVelsLarge(std::vector<Robot*>&);

private:
    QUdpSocket udpsocket;
    QHostAddress _addr;
    quint16 _port;

    //! @brief Sends a packet to the simulator
    void sendPacket(int id, int LF, int RF, int LB, int RB, bool kick, bool drible);
};

#endif // SIMROBCOMM_H
