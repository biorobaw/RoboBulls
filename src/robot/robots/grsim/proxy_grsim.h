#ifndef SIMROBCOMM_H
#define SIMROBCOMM_H
#include <QtNetwork/QUdpSocket>
#include "robot/robcomm.h"


//! @brief Percentage of stored robot velocity sent to simulator
const float mappingRatio = 1.0;

/*! @brief RobComm to send to the grSim simulator
 * @details uses ProtoBuf and QT to send to a computer running a grSim simulator */

class RobCommGrsim : public RobComm
{
public:
    //! @brief Constructor sets the IP and port to send to **see .cpp**
    RobCommGrsim(YAML::Node* t_node);

    void sendVels(std::set<Robot*>&) override;

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
