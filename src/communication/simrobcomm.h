#ifndef SIMROBCOMM_H
#define SIMROBCOMM_H
#include <QtNetwork/QUdpSocket>
#include "src/model/robot.h"
#include "robcomm.h"

//! @brief Percentage of stored robot velocity sent to simulator
const float mappingRatio = 1.0;

/*! @brief RobComm to send to the grSim simulator
 * @details uses ProtoBuf and QT to send to a computer running a grSim simulator */

class SimRobComm : public RobComm
{
public:
    //! @brief Constructor sets the IP and port to send to **see .cpp**
    SimRobComm(const char* addr, int port);

    void sendVelsLarge(std::set<Robot*>&) override;

private:
    QUdpSocket udpsocket;
    QHostAddress _addr;
    quint16 _port;

    //! @brief Sends a standard packet to the simulator
    void sendPacket(Robot *robot);

    //! @brief Sends all replacement packets to the simulator
    void sendReplacementPackets();

    void close() override;
};

#endif // SIMROBCOMM_H
