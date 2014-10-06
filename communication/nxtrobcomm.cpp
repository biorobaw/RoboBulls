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

void NXTRobComm::sendVelsLarge(std::vector<Robot*>& robots)
{
    packet_t teamPacketBuf[5];

    assert(sizeof(int8_t) == 1);
    memset(&teamPacketBuf, 0, sizeof(packet_t)*5);

    for (Robot* rob : robots)
    {
        int id =  rob->getID();
        packet_t* idPacket = &teamPacketBuf[id];
        idPacket->tilde = '~';
        idPacket->id = id;
        idPacket->left_front  = Measurments::clamp(rob->getL(), -127, 127);
        idPacket->left_back   = Measurments::clamp(rob->getL(), -127, 127);
        idPacket->right_front = Measurments::clamp(rob->getR(), -127, 127);
        idPacket->right_back  = Measurments::clamp(rob->getR(), -127, 127);
        idPacket->kick = rob->getKick();
        idPacket->chip_power = 0;
        idPacket->dribble_power = rob->getDrible() ? 1 : 0;
        idPacket->dollar = '$';
    }

    send((char*)&teamPacketBuf, sizeof(packet_t)*5);
}
