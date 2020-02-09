///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
/// Modified by Robobulls
///----------------------------------------------------------------------------



#pragma platform(NXT)

int readId(){
	TFileHandle   hFileHandle;              // will keep track of our file
  TFileIOResult nIOResult;                // will store our IO results
  string        sFileName = "id.txt";   	// the name of our file
  int           nFileSize = 100;          // will store our file size


  char          incommingChar;                          // this will store each char as we read back in from the file
  int           nLineCounter = 0;                       // this will let us know which line we are on when reading and writing (used as the index to 'incommingString[]')

  if(!OpenRead(hFileHandle, nIOResult, sFileName, nFileSize)){
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

////////////////////////////////////////////////////////////////////////
// Steer the motors and kick
////////////////////////////////////////////////////////////////////////
void steerMotors(int steerL, int steerR, int steerB, int kick)
{
	motor[motorA] = steerL;
	motor[motorB] = steerR;
	motor[motorC] = steerB;

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
void displayVels(int steerL, int steerR, int steerB, int kick){
	nxtDisplayCenteredTextLine(3, "SteerL......%i",steerL,);
	nxtDisplayCenteredTextLine(4, "SteerR......%i",steerR,);
	nxtDisplayCenteredTextLine(5, "SteerB......%i",steerB,);
	//nxtDisplayCenteredTextLine(5, "Kick........%i",kick,);
}


////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
task main()
{

	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

	//Setup the link to be a high-speed link.
	setupHighSpeedLink();

	nMotorPIDSpeedCtrl[motorA] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[motorB] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[motorC] = mtrSpeedReg;

	//nMotorEncoder[motorA] = 0;

	// Get Kicker to known position
	//kickerToKnownPos();


	// State variable
	//  - w: waiting for message
	//  - i: waitinf for id
	//  - l: waiting for left vel
	//  - r: waiting for right vel
	//  - b: waiting for back vel
	//  - k: waiting for kicker vel
	char state = 'w';
	byte byteRead[1];
	int steerL = 0;    // Global variables for steering.
	int steerR = 0;
	int steerB = 0;
	int kick = 0;
	int prevSteerL = 0;    // Global variables for steering.
	int prevSteerR = 0;
	int prevSteerB = 0;
	int prevKick = 0;
	int id;
	int myId = readId();
	nxtDisplayCenteredTextLine(2, "ID: %d", myId);

	while (nNxtButtonPressed != kExitButton){
		// Array we'll be reading into.
		while (nxtGetAvailHSBytes() < 1) EndTimeSlice();

		if (nxtReadRawHS(&byteRead[0], 1)){             // Read the array.
			switch (state){
			case 'w':
				if(byteRead[0] == '~')
					state = 'i';
				break;
			case 'i':
				id = (int)(sbyte)byteRead[0];
				nxtDisplayCenteredTextLine(2, "Command ID: %i", id);
				state = 'l';
				break;
			case 'l':
				steerL = (int)(sbyte)byteRead[0];
				state = 'r';
				break;
			case 'r':
				steerR = (int)(sbyte)byteRead[0];
				state = 'b';
				break;
		  case 'b':
		  	steerB = (int)(sbyte)byteRead[0];
		  	state = 'k';
		  	break;
			case 'k':
				kick = (int)(sbyte)byteRead[0];
				if (id == myId){
					if ( steerL != prevSteerL || steerR != prevSteerR || steerB != prevSteerB || kick != prevKick)
						steerMotors(steerL, steerR, steerB, kick);
					prevSteerL = steerL;
					prevSteerR = steerR;
					prevSteerB = steerB;
					prevKick = kick;
					displayVels(steerL, steerR, steerB, kick);
				}
				state = 'w';
				break;
			}
		}
		wait1Msec(1);
	}
}// end task
