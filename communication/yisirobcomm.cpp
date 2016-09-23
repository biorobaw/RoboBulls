#include "yisirobcomm.h"
#include <bitset>
#include <QtDebug>

YisiRobComm::YisiRobComm()
{
    // Setup Serial Port
    serial.setPortName("/dev/yisibot");

    if (serial.open(QIODevice::ReadWrite))	{
        std::cout << "Serial Port Connected" << std::endl;
        serial.setBaudRate(QSerialPort::Baud115200);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        int frequency = 0; // 0~9

        QByteArray sp1 = QByteArrayLiteral("\xff\xb0\x01\x02\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x31");
        QByteArray sp2 = QByteArrayLiteral("\xff\xb0\x04\x05\x06\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
        sp2[5] = sp2[5] + frequency;
        sp2[24] = CCrc8::calc((unsigned char*)sp2.data(), 24); //CRC

        // Send Start Packets to Configure the transmitter
        serial.write(sp1,25);
        serial.write(sp2,25);
        serial.flush();
    }
    else
        std::cout << "Unable to Open YisiBot Serial Port" << std::endl;
}

void YisiRobComm::sendVelsLarge(std::vector<Robot*>& robots)
{
    // Send Transmit Packets
    Data8 transmitPacket[25] = {(Data8)0};

    for(Robot* r: robots)
    {
        if(r->getID() != 0)
            continue;

        transmitPacket[0] = (Data8)0xff;
        // First Robot ID
        int robotID = r->getID();
        if(robotID > 7) {
            transmitPacket[1] = (1<<(robotID-8))|0x00;
            transmitPacket[2] = 0x00;
        }
        else{
            transmitPacket[1]=0x00;
            transmitPacket[2]=1<<robotID;
        }

        // First Robot Kick, Chip, Dribble
        int shootMode = 0;
        int dribble = r->getDribble();
        int dribble_level = 3;
        transmitPacket[3] = (shootMode << 6);
        transmitPacket[3] = transmitPacket[3]|(dribble?(dribble_level<<4):0);

        // First Robot Motion
        int velX = r->getXVel();
        int velY = r->getYVel();
        int velR = r->getAngVel();

        transmitPacket[4] = ((velX >= 0)?0:0x80) | (abs(velX) & 0x7f);
        transmitPacket[5] = ((velY >= 0)?0:0x80) | (abs(velY) & 0x7f);
        transmitPacket[6] = ((velR >= 0)?0:0x80) | (abs(velR) & 0x7f);
        if(transmitPacket[4]==char(0xff)) transmitPacket[4] = 0xfe;
        if(transmitPacket[5]==char(0xff)) transmitPacket[5] = 0xfe;
        if(transmitPacket[6]==char(0xff)) transmitPacket[6] = 0xfe;

        transmitPacket[15]=((abs(velX)&0x180)>>1)|((abs(velY)&0x180)>>3)|((abs(velR)&0x180)>>5);
    }

    transmitPacket[0] = (Data8)0xff;
    // Set Robot ID
    int robotID = 0;
    if(robotID > 7) {
        transmitPacket[1] = (1<<(robotID-8))|0x00;
        transmitPacket[2] = 0x00;
    }
    else{
        transmitPacket[1]=0x00;
        transmitPacket[2]=1<<robotID;
    }

    // First Robot Kick, Chip, Dribble
    int shootMode = 0;
    int dribble = 0;
    int dribble_level = 3;
    transmitPacket[3] = (shootMode << 6);
    transmitPacket[3] = transmitPacket[3]|(dribble?(dribble_level<<4):0);

    // First Robot Motion
    int velX = 0;
    int velY = 0;
    int velR = 0;

    transmitPacket[4] = ((velX >= 0)?0:0x80) | (abs(velX) & 0x7f);
    transmitPacket[5] = ((velY >= 0)?0:0x80) | (abs(velY) & 0x7f);
    transmitPacket[6] = ((velR >= 0)?0:0x80) | (abs(velR) & 0x7f);
    if(transmitPacket[4]==char(0xff)) transmitPacket[4] = 0xfe;
    if(transmitPacket[5]==char(0xff)) transmitPacket[5] = 0xfe;
    if(transmitPacket[6]==char(0xff)) transmitPacket[6] = 0xfe;

    transmitPacket[15]=((abs(velX)&0x180)>>1)|((abs(velY)&0x180)>>3)|((abs(velR)&0x180)>>5);

    serial.write(transmitPacket,25);
    serial.flush();
}
