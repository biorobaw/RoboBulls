#ifndef YISIROBCOMM_H
#define YISIROBCOMM_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <iostream>
#include "model/robot/robot_implementation.h"


typedef char Data8;

class ProxyYisi : public RobotImplementation
{
    Q_OBJECT
public:
    ProxyYisi(YAML::Node* t_node);
    void sendCommands(const QSet<Robot*>&) override;

    bool hasKicker() override;
    bool isHolonomic() override;
    RobotPilot* createPilot(Robot* robot) override;

    QString getName() override;

private:
    QSerialPort	serial;

protected:
    void close() override;
};

#endif // YISICOMM_H
