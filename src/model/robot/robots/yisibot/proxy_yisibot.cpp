#include "proxy_yisibot.h"
#include <bitset>
#include <QThread>
//#include <unistd.h>
#include <ctime>
#include "model/robot/robot.h"
#include "model/game_state.h"
#include "model/team/behaviors/genericmovementbehavior.h"
#include "configuration/motion_parameters.h"
#include "model/ball.h"
#include "utilities/my_yaml.h"
#include "crc.h"
#include "model/robot/navigation/pilots/pilot_omni.h"

ProxyYisi::ProxyYisi(YAML::Node* t_node)
{
    qInfo() << "            USB_PORT      -" <<  (*t_node)["USB_PORT"]  ;
    qInfo() << "            FREQUENCY     -" <<  (*t_node)["FREQUENCY"] ;

    QString usb_port = (*t_node)["USB_PORT"].as<std::string>().c_str();
    int frequency = (*t_node)["FREQUENCY"].as<int>();


    // Setup Serial Port

    //Oroginal code
    serial.setPortName(usb_port);


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
        QThread::msleep(1000);

        std::cout << "-DBG: YisiBot Serial Port configured " << std::endl;

    }
    else std::cout << "ERROR: Unable to Open YisiBot Serial Port" << std::endl;
}

std::time_t t_old = 0;
void ProxyYisi::sendVels(const QSet<Robot*>& robots)
{
    if(!serial.isOpen()){
        qWarning() << "WARNING: speeds not sent, yisibot serial comm is closed" ;
        return;
    }

    // Send Transmit Packets
    std::time_t t = std::time(0);

    //std::cout << "time: "<< difftime(t,t_old);
    t_old = t;
    for(Robot* r: robots)
    {

        RobotLowLevelControls* controls = r->getActiveController();

        Data8 transmitPacket[25] = {(Data8)0};

        transmitPacket[0] = (Data8)0xff;
        int robotID = r->getId();


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
        int shootPowerLevel = controls->getKickSpeed()? 127:0;
        transmitPacket[3] = (shootMode << 6);
        transmitPacket[18]=(controls->getKickSpeed()?shootPowerLevel:0)&0x7f;
        controls->setKickSpeed(0);

        // First Robot Dribble
        int dribble = controls->getDribble();
        int dribble_level = 3;
        transmitPacket[3] = transmitPacket[3]|(dribble?(dribble_level<<4):0);
        //std::cout << "Dribble" << std::endl;

        // Retrieve Desired Velocities & set to appropriate units
        auto vel = controls->getTargetVelocity();
        int velX = vel.x;        // Yisicomm robots expect flipped X/Y axis
        int velY = vel.y;
        int velR = controls->getTargetAngularSpeed() * 750 ;        // Mult by 750 to bring vals inline with yisi units (1/40 radians/sec)
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
        QThread::msleep(4);

    }

}


void ProxyYisi::close(){
    qDebug() << " closing yisi serial port..." ;
    serial.close();
    qDebug() << " closed yisi serial port";
}


bool ProxyYisi::hasKicker() {
    return true;
}
bool ProxyYisi::isHolonomic() {
    return true;
}
Pilot* ProxyYisi::createPilot(Robot* robot) {
    return new PilotOmni(robot, 0.25, 0, 0.1, 0);
}

QString ProxyYisi::getName(){
    return "yisibot";
}
