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
    // Create array of packets
    packet_t teamPacketBuf[5];
    //char testOmni[10] = {'~','6','100','100','100','100','0','0','0','$'};

    assert(sizeof(int8_t) == 1);

    // Initialize packet to zeros
    memset(&teamPacketBuf, 0, sizeof(packet_t)*5);

    // For each robot
    for(unsigned i = 0; i != robots.size(); ++i)
    {
        // Load information into the packet
        packet_t* packet = &teamPacketBuf[i];
        Robot* rob =  robots[i];
        packet->tilde = '~';
        packet->id = rob->getID();
        packet->left_front  = Measurments::clamp(rob->getLF(), -127, 127);
        packet->left_back   = Measurments::clamp(rob->getLB(), -127, 127);
        packet->right_front = Measurments::clamp(rob->getRF(), -127, 127);
        packet->right_back  = Measurments::clamp(rob->getRB(), -127, 127);
        packet->kick = rob->getKick() ? 'k' : 0;
        packet->chip_power = 0;
        packet->dribble_power = rob->getDrible() ? 1 : 0;
        packet->dollar = '$';

        // Clear robot information
        rob->setL(0);
        rob->setR(0);
        rob->setKick(0);
        rob->setDrible(0);
    }

    // Send Array of packets
    send((char*)&teamPacketBuf, sizeof(packet_t)*5);
    //send(&testOmni[0], 10);
}
