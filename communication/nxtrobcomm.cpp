#include "nxtrobcomm.h"

NXTRobComm::NXTRobComm()
{
    int Ret; //used for return values

    // open serial port
    Ret=Xbee.Open("/dev/xbee",57600);

    if (Ret!=1) { // If an error occured...
        printf ("Error while opening port. Permission problem ?\n"); // ... display a message ...
        exit(1);
        //return Ret;// ... quit the application
    }
    else{
        printf ("Serial port opened successfully !\n");
    }
}

NXTRobComm::~NXTRobComm()
{
    Xbee.Close();
}

void NXTRobComm::send(unsigned b)
{
    Xbee.WriteChar(b);
}

void NXTRobComm::send(char* commptr, int size)
{
    Xbee.Write(commptr, size);
}

void NXTRobComm::sendVels(int leftVel, int rightVel, int robotId)
{
    char comm[5] = {'~', (char)robotId, (char)leftVel, (char)rightVel, 0};
    send(&comm[0], 5);
}

void NXTRobComm::sendKick(int robotId)
{
    char comm[5] = {'~', (char)robotId, 0, 0, 1};
    send(&comm[0], 5);
}

void NXTRobComm::sendVelsLarge(std::vector<Robot*>& robots)
{
    for (Robot* rob : robots)
    {
        switch(rob->type())
        {

        case differential:
            sendVelsDifferential(robots);
            break;
        case threeWheelOmni:
            sendVelsThreeOmni(rob->getL(),rob->getR(),rob->getB(),rob->getID());
            break;
        case fourWheelOmni:
            sendVelsFourOmni(rob->getLF(),rob->getRF(),rob->getLB(),rob->getRB(),rob->getID());
        }
    }
}

void NXTRobComm::sendVelsDifferential(std::vector<Robot*>& robots)
{
    char largePacket[robots.size()*5];

    for (unsigned int i=0; i < robots.size(); i++)
    {
        Robot *rob = robots[i];

        /* Protects 8-bit velocity values from overflow */
        rob->setL(Measurments::clamp(rob->getL(), -127, 127));
        rob->setR(Measurments::clamp(rob->getR(), -127, 127));

		largePacket[5*i] = '~';
		largePacket[5*i+1] = (char)rob->getID();
		largePacket[5*i+2] = (char)rob->getL();
		largePacket[5*i+3] = (char)rob->getR();
		largePacket[5*i+4] = (char)rob->getKick();
		
        rob->setKick(0);
    }
    send(&largePacket[0], robots.size()*5);
}

void NXTRobComm::sendVelsThreeOmni(int left, int right, int back, int ID)
{

    char comm[6] = {'~', (char)4, (char)left, (char)right, (char)back, char(0)};
    //char comm[6] = {'~', (char)4, (char)0, (char)0, (char)0, char(0)};
    send(&comm[0], 6);
}

void NXTRobComm::sendVelsFourOmni(int front_left, int front_right, int rear_left, int rear_right, int ID)
{
    char comm[6] = {'~', (char)ID, (char)front_left, (char)front_right, (char)rear_left, (char)rear_right};
    send(&comm[0], 6);
}
