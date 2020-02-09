#ifndef YISIROBCOMM_H
#define YISIROBCOMM_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <iostream>
#include "src/model/robot.h"
#include "robcomm.h"
#include "crc.h"

typedef char Data8;

class YisiRobComm : public RobComm
{
public:
    YisiRobComm(std::string usb_port, int frequency);
    void sendVelsLarge(std::set<Robot*>&) override;
private:
    QSerialPort	serial;
    void close() override;
};

#endif // YISICOMM_H
