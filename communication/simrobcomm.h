#ifndef SIMROBCOMM_H
#define SIMROBCOMM_H
#include <QtNetwork/QUdpSocket>
#include "model/robot.h"
#include "robcomm.h"

//!< @brief Percentage of stored robot velocity sent to simulator
const float mappingRatio = 0.333;

/*! @brief RobComm to send to the grSim simulator
 * @details uses ProtoBuf and QT to send to a computer running a grSim simulator */

class SimRobComm : public RobComm
{
public:
    //! @brief Constructor sets the IP and port to send to **see .cpp**
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
