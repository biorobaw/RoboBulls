#ifndef PROXYRPI_2019_H
#define PROXYRPI_2019_H

//#include <QObject>
#include "model/robot/robot_implementation.h"
#include <QtNetwork/QUdpSocket>
#include "utilities/point.h"
#include <map>
#include <utility>


//class ProxyRpi_2019
//{
//public:
//    ProxyRpi_2019();
//};

//typedef char Data8;

class ProxyRpi_2019 : public RobotImplementation
{
    Q_OBJECT
public:
    ProxyRpi_2019(YAML::Node* t_node);
    void sendCommands(const QSet<Robot*>&) override;

    bool hasKicker() override;
    bool isHolonomic() override;
    RobotPilot* createPilot(Robot* robot) override;

    QString getName() override;

private:
//    OmniDrive drive;
    QUdpSocket* udpsocket = new QUdpSocket(this);
    QHostAddress _addr;
    quint16 _port;
    std::map<int, std::pair<QHostAddress, int>> ROBOT_ADDRS;
    //! @brief Sends a standard packet to the rpi_219
    void sendPacket(Robot *robot);

//    //! @brief Sends all replacement packets to the simulator
//    void sendReplacementPackets();


//    void getWheelSpeeds(Point velocity, float angular_speed, double wheelSpeeds[4]);

protected:
    void close() override;
};



#endif // PROXYRPI_2019_H
