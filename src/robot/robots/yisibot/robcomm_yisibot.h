#ifndef YISIROBCOMM_H
#define YISIROBCOMM_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <iostream>
#include "robot/robot.h"
#include "robot/robcomm.h"
#include "crc.h"

typedef char Data8;

class YisiRobComm : public RobComm
{
public:
    YisiRobComm(YAML::Node t_node);
    void sendVels(std::set<Robot*>&) override;
private:
    QSerialPort	serial;
    void close() override;
};

#endif // YISICOMM_H
