#include <cstring>
#include "nxtrobcomm.h"
#include "utilities/debug.h"
#include "include/config/robot_types.h"

//Used for testing wheels; see ROBOT_WHEEL_TEST
float lf, lb, rf, rb;

//A constant to multiple velocities
static float k = 0.4;

NXTRobComm::NXTRobComm()
{
    if (Xbee.Open("/dev/xbee", 57600) != 1) {
        printf ("Error while opening port. Permission problem ?\n");
        exit(1);
    } else {
        printf ("Serial port opened successfully !\n");
    }

    //Register variables for wheel tests
    lf = lb = rf = rb = 0;
    debug::registerVariable("lf", &lf);
    debug::registerVariable("lb", &lb);
    debug::registerVariable("rf", &rf);
    debug::registerVariable("rb", &rb);
}

NXTRobComm::~NXTRobComm()
{
    Xbee.Close();
}

//Config to change sending individual wheels or program velocities
#define ROBOT_WHEEL_TEST 0

void NXTRobComm::sendVelsLarge(std::vector<Robot*>& robots)
{
    // Create array of packets
    packet_t teamPacketBuf[5];

    // Initialize packet to zeros
    std::memset(&teamPacketBuf, 0, sizeof(packet_t)*5);

    // For each robot...
    for(unsigned i = 0; i != robots.size(); ++i)
    {
        // Load information into the packet
        packet_t* packet = &teamPacketBuf[i];
        Robot* rob =  robots[i];
        packet->id = rob->getID();

        if(rob->type() == fourWheelOmni) {
            //Packet format with Arduino: 250 and 255 with k+100, Kick is 1/0
            packet->tilde = char(250);
            packet->dollar = char(255);
        #if ROBOT_WHEEL_TEST
            packet->left_front  = lf + 100;
            packet->left_back   = lb + 100;
            packet->right_front = rf + 100;
            packet->right_back  = rb + 100;
        #else
            packet->left_front  = Measurments::clamp(rob->getLF(), -127, 127)*k + 100;
            packet->left_back   = Measurments::clamp(rob->getLB(), -127, 127)*k + 100;
            packet->right_front = Measurments::clamp(rob->getRF(), -127, 127)*k + 100;
            packet->right_back  = Measurments::clamp(rob->getRB(), -127, 127)*k + 100;
        #endif
            packet->kick = rob->getKick() ? 1 : 0;
        }
        else {
            //Packet format with lego NXTs, use ~ and $ with no K nor +100, Kick is 'k'
            packet->tilde = '~';
            packet->dollar = '$';
            packet->left_front  = Measurments::clamp(rob->getLF(), -127, 127);
            packet->left_back   = Measurments::clamp(rob->getLB(), -127, 127);
            packet->right_front = Measurments::clamp(rob->getRF(), -127, 127);
            packet->right_back  = Measurments::clamp(rob->getRB(), -127, 127);
            packet->kick = rob->getKick() ? 'k' : 0;
        }

        //Dribble (1/0) and Chip power (no chipper)
        packet->chip_power = 0;
        packet->dribble_power = rob->getDrible();

        //Reset kick and dribble status in robot
        rob->setKick(0);
        rob->setDrible(0);
    }

    // Send Array of packets
    Xbee.Write((char*)&teamPacketBuf, sizeof(packet_t)*5);
}
