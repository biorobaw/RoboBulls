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
    bool packet_sent_diff = 0;
    bool packet_sent_three = 0;
    bool packet_sent_four = 0;

    for (Robot* rob : robots)
    {
        switch(rob->type())
        {

        case differential:
            if (!packet_sent_diff /*&& !sent*/)
                sendVelsDifferential(robots);
            packet_sent_diff = 1;
            sent = 1;
            break;
        case threeWheelOmni:
            if (!packet_sent_three)
                sendVelsThreeOmni(rob->getL(),rob->getR(),rob->getB(),rob->getID());
            packet_sent_three = 1;
            break;
        case fourWheelOmni:
            if (!packet_sent_four)
                sendVelsFourOmni(rob->getLF(),rob->getRF(),rob->getLB(),rob->getRB(),rob->getID());
            packet_sent_four = 1;
        }
    }
}

void NXTRobComm::sendVelsDifferential(std::vector<Robot*>& robots)
{
    cout << "sending" << endl;
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

//        if (rob->getKick() != 1)
//            cout << "trouble" << endl;
		
        rob->setKick(0);
    }
//    char a[50];
//    for (int i = 0; i < 50; i++)
//    {
//        a[i] = (char)(i+1);
//    }
//    a[0] = '1';
//    a[1] = '2';
//    a[2] = '3';
//    a[3] = '4';
//    a[4] = '5';
//    a[5] = '6';
//    a[6] = '7';
//    a[7] = '8';
//    a[8] = '9';
//    a[9] = '10';
//    a[10] = '11';
//    a[11] = '12';
//    a[12] = '13';
//    a[13] = '14';
//    a[14] = '15';
//    a[15] = '16';
//    a[16] = '17';
//    a[17] = '18';
//    a[18] = '19';
//    a[19] = '20';
//    a[20] = '21';
//    a[21] = '22';
//    a[22] = '23';
//    a[23] = '24';
//    a[24] = '25';
//    a[25] = '26';
//    a[26] = '27';
//    a[27] = '28';
//    a[28] = '29';
//    a[29] = '30';
//    a[30] = '31';
//    a[31] = '32';
//    a[32] = '33';
//    a[33] = '34';
//    a[34] = '35';
//    a[35] = '36';
//    a[36] = '37';
//    a[37] = '38';
//    a[38] = '39';
//    a[39] = '40';
//    a[40] = '41';
//    a[41] = '42';
//    a[42] = '43';
//    a[43] = '44';
//    a[44] = '45';
//    a[45] = '46';
//    a[46] = '47';
//    a[47] = '48';
//    a[48] = '49';
//    a[49] = '50';

//    send(&a[0], 50);
    send(&largePacket[0], robots.size()*5);
}

void NXTRobComm::sendVelsThreeOmni(int left, int right, int back, int ID)
{
    char comm[6] = {'~', (char)ID, (char)left, (char)right, (char)back, char(0)};
    //char comm[6] = {'~', (char)4, (char)0, (char)0, (char)0, char(0)};
    send(&comm[0], 6);
}

void NXTRobComm::sendVelsFourOmni(int front_left, int front_right, int rear_left, int rear_right, int ID)
{
    char comm[6] = {'~', (char)ID, (char)front_left, (char)front_right, (char)rear_left, (char)rear_right};
    send(&comm[0], 6);
}
