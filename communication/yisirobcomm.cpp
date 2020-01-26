#include "yisirobcomm.h"
#include <bitset>
#include <unistd.h>
#include <ctime>
#include "model/robot.h"
#include "model/gamemodel.h"
#include "behavior/genericmovementbehavior.h"
#include "parameters/motion_parameters.h"
#include "model/ball.h"

YisiRobComm::YisiRobComm(std::string usb_port, int frequency)
{
    // Setup Serial Port

    //Oroginal code
    //serial.setPortName("/dev/ttyUSB2");
    serial.setPortName(usb_port.c_str());
//    serial.setBaudRate(QSerialPort::Baud115200);
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.open(QIODevice::ReadWrite))	{
        serial.setBaudRate(QSerialPort::Baud115200);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        QByteArray sp1 = QByteArrayLiteral("\xff\xb0\x01\x02\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x31");
        QByteArray sp2 = QByteArrayLiteral("\xff\xb0\x04\x05\x06\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
        sp2[5] = sp2[5] + frequency;
        sp2[24] = CCrc8::calc((unsigned char*)sp2.data(), 24); //CRC
        //std::cout << "Open YisiBot Serial Port" << std::endl;
        // Send Start Packets to Configure the transmitter
        serial.write(sp1.data(),25);
        serial.write(sp2.data(),25);
        serial.flush();



        //while(serial.bytesAvailable()){
        //    std::cout << "Waiting for bytes to be written" << std::endl;
        //    serial.waitForBytesWritten(1000);
        //}
        usleep(1000000);    //us

        std::cout << "-DBG: YisiBot Serial Port configured " << std::endl;

    }
    else std::cout << "ERROR: Unable to Open YisiBot Serial Port" << std::endl;
}

std::time_t t_old = 0;
void YisiRobComm::sendVelsLarge(std::set<Robot*>& robots)
{
    if(!serial.isOpen()){
        std::cout << "WARNING: speeds not sent, yisibot serial comm is closed" << std::endl;
        return;
    }

    // Send Transmit Packets
    std::time_t t = std::time(0);

    //std::cout << "time: "<< difftime(t,t_old);
    t_old = t;
    for(Robot* r: robots)
    {


        Data8 transmitPacket[25] = {(Data8)0};

        Point bp = Ball::getPosition();
//        std::cout<<"Ball point is at: "<<bp.toString()<<std::endl;

        transmitPacket[0] = (Data8)0xff;
        // First Robot ID
        int robotID = r->getID();
//        std::cout<< "-YISI: sending to " << r->getID() << std::endl;

        float angleToBall = Measurements::angleBetween(r, bp);
//        std::cout<<"Ball angle is at: "<<angleToBall<<std::endl;


        if(robotID > 7) {
            transmitPacket[1] = (1<<(robotID-8))|0x00;
            transmitPacket[2] = 0x00;
        }
        else{
            transmitPacket[1]=0x00;
            transmitPacket[2]=1<<robotID;
        }

        // First Robot Kick/Chip (0/1)
        int shootMode = 0;
        //std::cout << "Kick66666" << std::endl;
        int shootPowerLevel = r->getKick()? 127:0;
        transmitPacket[3] = (shootMode << 6);
        transmitPacket[18]=(r->getKick()?shootPowerLevel:0)&0x7f;
        r->setKick(0);

        // First Robot Dribble
        int dribble = r->getDribble();
        int dribble_level = 3;
        transmitPacket[3] = transmitPacket[3]|(dribble?(dribble_level<<4):0);
        //std::cout << "Dribble" << std::endl;

        // Retrieve Desired Velocities & set to appropriate units
        int velX = r->getYVel();        // Yisicomm robots expect flipped X/Y axis
        int velY = r->getXVel();
        int velR = -r->getAngVel() * 750 ;        // Mult by 750 to bring vals inline with yisi units (1/40 radians/sec)
//        std::cout << "velx/y/ang  " << velX << "  " << velY << "  " << velR << std::endl;

        transmitPacket[4] = ((velX >= 0)?0:0x80) | (abs(velX) & 0x7f);
        transmitPacket[5] = ((velY >= 0)?0:0x80) | (abs(velY) & 0x7f);
        transmitPacket[6] = ((velR >= 0)?0:0x80) | (abs(velR) & 0x7f);
        if(transmitPacket[4]==char(0xff)) transmitPacket[4] = 0xfe;
        if(transmitPacket[5]==char(0xff)) transmitPacket[5] = 0xfe;
        if(transmitPacket[6]==char(0xff)) transmitPacket[6] = 0xfe;
        transmitPacket[15]=((abs(velX)&0x180)>>1)|((abs(velY)&0x180)>>3)|((abs(velR)&0x180)>>5);
        serial.write(transmitPacket,25);
        serial.flush();
        usleep(4000);

    }
    //std::cout << "at YisiRobComm::sendVelsLarge(std::vector<Robot*>& robots) 2 \n";

    //std::cout << "at YisiRobComm::sendVelsLarge(std::vector<Robot*>& robots) 3 \n";

    //std::cout << "at YisiRobComm::sendVelsLarge(std::vector<Robot*>& robots) 4 \n";
    //std::cout << "Sent" << std::endl;
}


void YisiRobComm::close(){
    std::cout << " closing yisi serial port..." << std::endl;
    serial.close();
    std::cout << " closed yisi serial port" << std::endl;
}
