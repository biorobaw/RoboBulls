#include "nxtrobcomm.h"
#include "gui/guiinterface.h"
#include "utilities/debug.h"
#include "include/config/robot_types.h"

//Used to control Omni's PID Controller
static float k = 0.4;
static int p = 30;
static int i = 10;

int a, b, c, d;

NXTRobComm::NXTRobComm()
{
    if (Xbee.Open("/dev/xbee",57600) != 1) {
        printf ("Error while opening port. Permission problem ?\n");
        exit(1);
    } else {
        printf ("Serial port opened successfully !\n");
    }
    a = b = c = d = 0;
    debug::registerVariable("Kp", &p);
    debug::registerVariable("Ki", &i);
    debug::registerVariable("lf", &a);
    debug::registerVariable("lb", &b);
    debug::registerVariable("rf", &c);
    debug::registerVariable("rb", &d);
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

//Config to change sending individual wheels or program velocities
#define ROBOT_WHEEL_TEST 0

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
        packet->id = rob->getID();

        if(rob->type() == fourWheelOmni) {
            //Packet format with Arduino: 250 and 255 with k+100, Kick is 1/0
            packet->tilde = char(250);
            packet->dollar = char(255);
        #if ROBOT_WHEEL_TEST
            packet->left_front  = a + 100;
            packet->left_back   = b + 100;
            packet->right_front = c + 100;
            packet->right_back  = d + 100;
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

        //Unused; p and i are hacks for the Encoder Arduino robot
        packet->chip_power = p;
        packet->dribble_power = i;

        //Reset kick and dribble status in robot
        rob->setKick(0);
        rob->setDrible(0);
    }

    // Send Array of packets
    send((char*)&teamPacketBuf, sizeof(packet_t)*5);
}
