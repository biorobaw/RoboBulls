///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
/// Modified by Robobulls
///----------------------------------------------------------------------------

#pragma platform(NXT)

#if 0
int readId()
{
	TFileHandle   hFileHandle;          // will keep track of our file
	TFileIOResult nIOResult;            // will store our IO results
	string        sFileName = "id.txt"; // the name of our file
	int           nFileSize = 100;      // will store our file size
	char          incommingChar;        // this will store each char as we read back in from the file
	int           nLineCounter = 0;     // this will let us know which line we are on when reading and writing (used as the index to 'incommingString[]')

	if(!OpenRead(hFileHandle, nIOResult, sFileName, nFileSize)){
		OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);
		WriteByte(hFileHandle, nIOResult, 1);         // write 'sMessageToWrite' to the file
		Close(hFileHandle, nIOResult);
		OpenRead(hFileHandle, nIOResult, sFileName, nFileSize);
	}

	ReadByte(hFileHandle, nIOResult, incommingChar);

	return (int) (ubyte) incommingChar;
}
#endif

int readId(){
	TFileHandle   hFileHandle;              // will keep track of our file
	TFileIOResult nIOResult;                // will store our IO results
	char*         sFileName = "id.txt";   // the name of our file
	short         nFileSize = 500;          // will store our file size
	char          incommingChar;                          // this will store each char as we read back in from the file
	int           nLineCounter = 0;                       // this will let us know which line we are on when reading and writing (used as the index to 'incommingString[]')

	if(!OpenRead(hFileHandle, nIOResult, sFileName, nFileSize)) {
		OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);
		WriteByte(hFileHandle, nIOResult, 1);         // write 'sMessageToWrite' to the file
		Close(hFileHandle, nIOResult);
		OpenRead(hFileHandle, nIOResult, sFileName, nFileSize);
	}

	ReadByte(hFileHandle, nIOResult, incommingChar);

	return (int) (ubyte) incommingChar;
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
	nxtDisplayCenteredTextLine(4, "SteerL......%i",steerL,);
	nxtDisplayCenteredTextLine(5, "SteerR......%i",steerR,);
	nxtDisplayCenteredTextLine(6, "Kick........%i",kick,);
}


////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
void readBlock(byte* block, int n)
{
	for (int x = 0; x < n; x++)
	{
		nxtReadRawHS(&block[x], 1);
	}
}


struct dataPacket {
	byte id;   					//Robot ID
	byte left_front;    //LF wheel velocity
	byte left_back;     //LB wheel velocity
	byte right_front;   //RF wheel velocity
	byte right_back;    //RB wheel velocity
	byte kick;          //Kick? 1/0
	byte chip_power;    //Chip kick power
	byte dribble_power; //Dribbler power
};
/* We recieve 5 of these^ in a single packet for the whole team. */


task main()
{
	// High Speed Link
	setup();

	// Display ID
	int myId = readId();
	nxtDisplayCenteredTextLine(1, "ID: %d", myId);


	byte current_block[10];
	dataPacket robot_info;

	// State machine
	// t = check for tilde
	// b = read bytes
	// d = check for dollar
	char state = 't';

	while (nNxtButtonPressed != kExitButton)
	{
		//Wait for packets
		while (nxtGetAvailHSBytes() < 1)
			EndTimeSlice();

		// If packets are detected
		switch (state)
		{
		case 't':
			nxtReadRawHS(&current_block[0], 2)
			if(current_block[0] == '~' && current_block[1] == myId)
				state = 'b';
			break;

		case 'b':
			readBlock(&current_block[2],8);
			state = 'd';
			break;

		case 'd':
			if(current_block[9] == '$')
			{
				//nxtDisplayCenteredTextLine(1, "%c",current_block[0],);
				//nxtDisplayCenteredTextLine(2, "%d", current_block[1],);
				//nxtDisplayCenteredTextLine(3, "%d", current_block[2],);
				//nxtDisplayCenteredTextLine(4, "%d", current_block[3],);
				//nxtDisplayCenteredTextLine(5, "%d", current_block[4],);
				//wait1Msec(2000);
				steerMotors(current_block[2], current_block[4], current_block[6]);
				displayVels(current_block[2], current_block[4], current_block[6]);
			}
			else
			{
				nxtDisplayCenteredTextLine(3,"Block Incomplete");
				//nxtDisplayCenteredTextLine(1, "%c",current_block[0],);
				//nxtDisplayCenteredTextLine(2, "%d", current_block[1],);
				//nxtDisplayCenteredTextLine(3, "%d", current_block[2],);
				//nxtDisplayCenteredTextLine(4, "%d", current_block[3],);
				//nxtDisplayCenteredTextLine(5, "%d", current_block[4],);
				//wait1Msec(2000);
			}
			state = 't';
		}

	}
}// end task
