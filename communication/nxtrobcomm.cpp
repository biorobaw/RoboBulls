#include "nxtrobcomm.h"
#include "gui/guiinterface.h"
#include "utilities/debug.h"

//Used to control Omni's PID Controller
static float k = 0.4;
static int p = 30;
static int i = 10;


NXTRobComm::NXTRobComm()
{
    if (Xbee.Open("/dev/xbee",57600) != 1) {
        printf ("Error while opening port. Permission problem ?\n");
        exit(1);
    } else {
        printf ("Serial port opened successfully !\n");
    }

    debug::registerVariable("Kp", &p);
    debug::registerVariable("Ki", &i);
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

    // Initialize packet to zeros
    memset(&teamPacketBuf, 0, sizeof(packet_t)*5);

    // For each robot
    for(unsigned i = 0; i != robots.size(); ++i)
    {
        // Load information into the packet
        packet_t* packet = &teamPacketBuf[i];
        Robot* rob =  robots[i];
        packet->tilde = char(250);
        packet->id = rob->getID();
        packet->left_front  = Measurments::clamp(rob->getLF(), -127, 127)*k + 100;
        packet->left_back   = Measurments::clamp(rob->getLB(), -127, 127)*k + 100;
        packet->right_front = Measurments::clamp(rob->getRF(), -127, 127)*k + 100;
        packet->right_back  = Measurments::clamp(rob->getRB(), -127, 127)*k + 100;
        packet->kick = rob->getKick() ? 1 : 0;
        packet->chip_power = p;
        packet->dribble_power = i;
        packet->dollar = char(255);

        // Ryan overriding the zero-reset in the event of a manual override from the GUI
        if (!GuiInterface::getGuiInterface()->isOverriddenBot()[robots[i]->id]) {
            // Clear robot information
            rob->setL(0);
            rob->setR(0);
            rob->setKick(0);
            rob->setDrible(0);
        }
    }

    // Send Array of packets
    send((char*)&teamPacketBuf, sizeof(packet_t)*5);
}
