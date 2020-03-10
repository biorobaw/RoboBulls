///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
/// Modified by Robobulls
///----------------------------------------------------------------------------

#pragma platform(NXT)

int readId()
{
    TFileHandle   hFileHandle;          // will keep track of our file
    TFileIOResult nIOResult;            // will store our IO results
    string        sFileName = "id.txt"; // the name of our file
    short         nFileSize = 100;      // will store our file size
    char          incommingChar;        // this will store each char as we read back in from the file

    if(!OpenRead(hFileHandle, nIOResult, sFileName, nFileSize)){
        OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);
        WriteByte(hFileHandle, nIOResult, 1);         // write 'sMessageToWrite' to the file
        Close(hFileHandle, nIOResult);
        OpenRead(hFileHandle, nIOResult, sFileName, nFileSize);
    }

    ReadByte(hFileHandle, nIOResult, incommingChar);

    return (int) (ubyte) incommingChar;
}

////////////////////////////////////////////////////////////////////////
// Set the kicker in a known position
////////////////////////////////////////////////////////////////////////
void kickerToKnownPos(){
    motor[motorA] = 20;
    int prevEnc = nMotorEncoder[motorA];
    int enc;
    bool equal;
    do {
        wait1Msec(50);
        enc = nMotorEncoder[motorA];
        equal = (enc == prevEnc);
        prevEnc = enc;
    } while (!equal);
    motor[motorA] = 0;
}

/////////////////////////////////////////////////////
// Sets the speedlink up
////////////////////////////////////////////////////
void setupHighSpeedLink()
{
    // Initialize port S$ to "high speed" mode.
    nxtEnableHSPort();
    // XBees must be preset to 115200 on beforehand
    // And also execute this function
    nxtSetHSBaudRate(57600);
    nxtHS_Mode = hsRawMode;
    //nxtHS_Mode = hsMsgModeMaster;

    return;
}

void setup()
{
    setupHighSpeedLink();

    eraseDisplay();
    bNxtLCDStatusDisplay = true; // Enable top status line display

    //Setup the link to be a high-speed link.
    setupHighSpeedLink();

    nMotorPIDSpeedCtrl[motorB] = mtrSpeedReg;
    nMotorPIDSpeedCtrl[motorC] = mtrSpeedReg;
    nMotorPIDSpeedCtrl[motorA] = mtrSpeedReg;
    nMotorEncoder[motorA] = 0;

    // Get Kicker to known position
    kickerToKnownPos();
}

void setup()
{
	setupHighSpeedLink();

	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

	//Setup the link to be a high-speed link.
	setupHighSpeedLink();

	nMotorPIDSpeedCtrl[motorB] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[motorC] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[motorA] = mtrSpeedReg;
	nMotorEncoder[motorA] = 0;

	// Get Kicker to known position
	kickerToKnownPos();
}

////////////////////////////////////////////////////////////////////////
// Steer the motors and kick
////////////////////////////////////////////////////////////////////////
void steerMotors(int steerL, int steerR, int kick)
{
    motor[motorB] = steerR;
    motor[motorC] = steerL;


    ////Kicking////
    if(kick == 1){
        nMotorEncoderTarget[motorA] = -100;
        motor[motorA] = -100;
        while(nMotorRunState[motorA] != runStateIdle){
        }
        nMotorEncoderTarget[motorA] = 100;
        motor[motorA] = 50;
        while(nMotorRunState[motorA] != runStateIdle){
        }

        kickerToKnownPos();
    }
}

////////////////////////////////////////////////////////////////////////
// Display the velocities in the LCD screen
////////////////////////////////////////////////////////////////////////
void displayVels(int steerL, int steerR, int kick){
    nxtDisplayCenteredTextLine(2, "Reading Bytes");
    nxtDisplayCenteredTextLine(4, "SteerL......%i",steerL,);
    nxtDisplayCenteredTextLine(5, "SteerR......%i",steerR,);
    nxtDisplayCenteredTextLine(6, "Kick........%i",kick,);
}


////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
/*
 * ** RBKicker5 Packet Format **
 * This is the standard packet format that is expected by all robots.
 * Our program sends a batch of 5 of these for the entire team at once.
 *
 * byte tilde           Always a tilde (~)
 * byte id              Robot ID
 * byte left_front      LF wheel velocity
 * byte left_back       LB wheel velocity
 * byte right_front     RF wheel velocity
 * byte right_back      RB wheel velocity
 * byte kick;           Kick? 1/0
 * byte chip_power      Chip kick power
 * byte dribble_power   Dribbler power
 * byte dollar          Always a dollar ('$')
 */

task main()
{
    setup();
    int myId = readId();
    nxtDisplayCenteredTextLine(2, "ID: %d", myId);
    int steerL = 0;
    int steerR = 0;
    int kick = 0;
    char state = 'i';
    byte packetBytes[10];

    PlaySound(soundShortBlip);

    while (nNxtButtonPressed != kExitButton)
    {
        while (nxtGetAvailHSBytes() < sizeof(byte)*5) EndTimeSlice();

        switch(state)
        {
        case 't':
            if(nxtReadRawHS(&packetBytes[0], 1)) {
                if(packetBytes[0] == '~') {
                    state = 'i';
                }
            }
            break;
        case 'i':
            if(nxtReadRawHS(&packetBytes[1], 1)) {
                if(packetBytes[1] == myId) {
                    state = 'b';
                    break;
                }
            }
            state = 't';
            break;
        case 'b':
            if(nxtReadRawHS(&packetBytes[2], 8*sizeof(byte))) {
                if(packetBytes[9] == '$') {
                    nxtDisplayClearTextLine(3);
                    state = 's';
                } else {
                    nxtDisplayCenteredTextLine(3, "Read Packet Incorrectly");
                    memset(packetBytes, 0, 10*sizeof(byte));
                    state = 't';
                }
                break;
            }
            state = 't';
            break;
        case 's':
            steerL = packetBytes[2];
            steerR = packetBytes[4];
            kick   = packetBytes[6];
            steerMotors(steerL, steerR, kick);
            displayVels(steerL, steerR, kick);
            state = 't';
            break;
        }
    }
}// end task
