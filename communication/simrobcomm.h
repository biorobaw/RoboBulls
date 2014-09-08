#ifndef SIMROBCOMM_H
#define SIMROBCOMM_H

#include <QtNetwork>
#include <math.h>

#include "model/robot.h"
#include "include/grSim_Packet.pb.h"
#include "include/grSim_Commands.pb.h"
#include "include/grSim_Replacement.pb.h"
#include "robcomm.h"

const float mappingRatio = 30;

class SimRobComm : public RobComm
{
public:
    SimRobComm();

    virtual void sendVelsLarge(std::vector <Robot*>);

    virtual void sendVelsThreeOmni(int left,int right, int back, int ID);

    void sendKick(int robotId);

private:
    QUdpSocket udpsocket;
    QHostAddress _addr;
    quint16 _port;

    void sendVels(int leftVel, int rightVel, int robotId);
    void sendPacket(int id, int leftVel, int rightVel, bool kick, bool drible);
};

#endif // SIMROBCOMM_H
