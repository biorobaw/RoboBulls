#include "nxtrobcomm.h"
#include <stdio.h>

NXTRobComm::NXTRobComm()
{
    int Ret; //used for return values

    // open serial port
    Ret=Xbee.Open("/dev/xbee",9600);

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
    char comm[5] = {'~', robotId, leftVel, rightVel, 0};
    send(&comm[0], 5);
}

void NXTRobComm::sendKick(int robotId)
{
    char comm[5] = {'~', robotId, 0, 0, 1};
    send(&comm[0], 5);
}
