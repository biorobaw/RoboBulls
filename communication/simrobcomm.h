#ifndef SIMROBCOMM_H
#define SIMROBCOMM_H

#include <QtNetwork>
#include "include/grSim_Packet.pb.h"
#include "include/grSim_Commands.pb.h"
#include "include/grSim_Replacement.pb.h"

#include "robcomm.h"

const float mappingRatio = 50;

class SimRobComm : public RobComm
{
public:
    SimRobComm();
    void sendVels(int leftVel, int rightVel, int robotId);
    void sendKick(int robotId);

private:
    QUdpSocket udpsocket;
    QHostAddress _addr;
    quint16 _port;

    void sendPacket(int id, int leftVel, int rightVel, bool kick);
};

#endif // SIMROBCOMM_H
