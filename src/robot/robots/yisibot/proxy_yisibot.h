#ifndef YISIROBCOMM_H
#define YISIROBCOMM_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <iostream>
#include "robot/robot_proxy.h"


typedef char Data8;

class ProxyYisi : public RobotProxy
{
public:
    ProxyYisi(YAML::Node* t_node);
    void sendVels(const QSet<Robot*>&) override;

    bool hasKicker() override;
    bool isHolonomic() override;
    Pilot* createPilot(Robot* robot) override;

private:
    QSerialPort	serial;
    void close() override;
};

#endif // YISICOMM_H
