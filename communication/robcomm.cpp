#include "robcomm.h"
#include <stdio.h>

robComm* robComm::nxtbee = NULL;

robComm::robComm()
{
    int Ret; //used for return values

    // open serial port
    Ret=Xbee.Open("/dev/xbee",9600);

    if (Ret!=1) { // If an error occured...
        printf ("Error while opening port. Permission problem ?\n"); // ... display a message ...
        //return Ret;// ... quit the application
    }
    else{
        printf ("Serial port opened successfully !\n");
    }
}

robComm::~robComm()
{
    Xbee.Close();
}

void robComm::send(unsigned b)
{
    Xbee.WriteChar(b);
}

void robComm::send(char* commptr, int size)
{
    Xbee.Write(commptr, size);
}

void robComm::sendVels(int leftVel, int rightVel, int robotId)
{
    char comm[5] = {'~', robotId, leftVel, rightVel, 0};
    send(&comm[0], 5);
}

void robComm::sendKick(int robotId)
{
    char comm[5] = {'~', robotId, 0, 0, 1};
    send(&comm[0], 5);
}

robComm * robComm::getnxtbee()
{
    if (nxtbee == NULL)
        nxtbee = new robComm();

    return nxtbee;
}

//void robComm::sendVels(int id, int rVel, int lVel)
//{
//    char comm[13] = {'~', 0,0,0,0,8,500,500,0,0,0,0,0};
//}
