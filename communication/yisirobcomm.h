#ifndef YISIROBCOMM_H
#define YISIROBCOMM_H

#include <QtSerialPort/QSerialPort>
#include <iostream>
#include "model/robot.h"
#include "robcomm.h"
#include "crc.h"

typedef char Data8;

class YisiRobComm : public RobComm
{
public:
    YisiRobComm();
    virtual void sendVelsLarge(std::vector<Robot*>&) override;
private:
    QSerialPort	serial;
};

#endif // YISICOMM_H
