/****************************************\
 * PID Feedback Motor Control
 * R. Mattes
 * Penn State Robotics Club
 * www.psurobotics.org
 * Modified for Robobulls by Muhaimen Shamsi
 \****************************************/

#include <Encoder.h>
#include <Servo.h>

#define ENCODER_OPTIMIZE_INTERRUPTS

int id;
int myid = 8;    // This Robot's ID

// Define pin locations for wheel encoding
#define LFEncPinA 2
#define LFEncPinB A0
#define LBEncPinA 20
#define LBEncPinB A2
#define RFEncPinA 3
#define RFEncPinB A1
#define RBEncPinA 21
#define RBEncPinB A3

// Define pin locations for kicker and chipper
#define kickPin 8

// Define an approximate constant for conversion between encoder ticks and speed
double Kvel = 0.09;
int maxPID = 20;

// PID Gains
double Kp = 3.0;
double Ki = 1.0;
double Kd = 0.0;
double maxint = 10.0;

// Velocities
//  90 = Stop
// 180 = Full Forward 
//   0 = Full Reverse
double targetLFvel = 90;
double targetRFvel = 90;
double targetLBvel = 90;
double targetRBvel = 90;

double LFvel = 90;
double RFvel = 90;
double LBvel = 90;
double RBvel = 90;

// Declare motor servos
Servo motorLF;   
Servo motorLB;   
Servo motorRF;   
Servo motorRB;

// PID weighted sums
double LFPID = 0;
double LBPID = 0;
double RFPID = 0;
double RBPID = 0;

// Encoder trackers for velocity calculations
volatile int LFEncPos = 0;
volatile int LBEncPos = 0;                                                                                                                                                                                                                                                                                                                                                                 
volatile int RFEncPos = 0;
volatile int RBEncPos = 0;
int LFEncPosPrev = 0;
int LBEncPosPrev = 0;
int RFEncPosPrev = 0;
int RBEncPosPrev = 0;

// Kicker Chipper Dribbler
int kick, chip, dribble = 0;

//***********************************************************************************
void setup()
{   
  // Set Kicker and Chipper pins
  pinMode(kickPin, OUTPUT);

  // Attach Motors
  motorLF.attach(6);
  motorLB.attach(9);
  motorRF.attach(10);  
  motorRB.attach(11);

  //CalibrateESC();
  writeAll(90);
  delay(3000);

  //Note: For the UNO, the USB/Micro
  //switch must be in the micro position. 

  // Start Serial Port
  Serial.begin(57600);
}
//***********************************************************************************

//***********************************************************************************

long LastLoopUpdate = 0;
void loop()
{
  runComm();
  if(millis()-LastLoopUpdate > 10)
  {
    getSpeeds();
    PID();
    setSpeeds();
    setKick();
    LastLoopUpdate = millis(); 
  }
  
}

//***********************************************************************************


//***********************************************************************************
// Return speed (scaled using kvel)
// Define Encoders
Encoder LFEnc(LFEncPinA, LFEncPinB);
Encoder LBEnc(LBEncPinA, LBEncPinB);
Encoder RFEnc(RFEncPinA, RFEncPinB);
Encoder RBEnc(RBEncPinA, RBEncPinB);

void getSpeeds()
{
  // Update Encoder Position
  LFEncPos = LFEnc.read();
  LBEncPos = LBEnc.read();
  RFEncPos = RFEnc.read();
  RBEncPos = RBEnc.read();

  // If an integer overflow occurs, throw out the new value
  // Otherwise, scale the difference to get speed
  if(abs(LFEncPos - LFEncPosPrev) < 60000)
    LFvel = 90-Kvel*(LFEncPos - LFEncPosPrev);
  if(abs(LBEncPos - LBEncPosPrev) < 60000)
    LBvel = 90+Kvel*(LBEncPos - LBEncPosPrev);
  if(abs(RFEncPos - RFEncPosPrev) < 60000)
    RFvel = 90-Kvel*(RFEncPos - RFEncPosPrev);
  if(abs(RBEncPos - RBEncPosPrev) < 60000)
    RBvel = 90+Kvel*(RBEncPos - RBEncPosPrev);

  // Store info for next time
  LFEncPosPrev = LFEncPos;
  LBEncPosPrev = LBEncPos;
  RFEncPosPrev = RFEncPos;
  RBEncPosPrev = RBEncPos;    

}
//***********************************************************************************


//***********************************************************************************
// Calculates PID values, adds to the target velocity, and sends commands to motors
unsigned long LastPIDUpdate = 0;

// memory for I and D terms
double lastLFerror = 0;
double lastLBerror = 0;
double lastRFerror = 0;
double lastRBerror = 0;
double LFi = 0;
double LBi = 0;
double RFi = 0;
double RBi = 0;

void PID()
{
  // find errors, dt
  double LFerror = (double)targetLFvel-(double)LFvel;
  double LBerror = (double)targetLBvel-(double)LBvel;
  double RFerror = (double)targetRFvel-(double)RFvel;
  double RBerror = (double)targetRBvel-(double)RBvel;
  double dt = (micros() - LastPIDUpdate);  
  // convert to seconds
  dt /= 1000000.0;
  // integral
  LFi += LFerror * dt;
  LBi += LBerror * dt;
  RFi += RFerror * dt;
  RBi += RBerror * dt;
  // prevent integral windup
  LFi = constrain(LFi, -maxint, maxint);
  LBi = constrain(LBi, -maxint, maxint);
  RFi = constrain(RFi, -maxint, maxint);
  RBi = constrain(RBi, -maxint, maxint);
  // differential
  double LFd=(LFerror - lastLFerror)/dt;
  double LBd=(LBerror - lastLBerror)/dt;
  double RFd=(RFerror - lastRFerror)/dt;
  double RBd=(RBerror - lastRBerror)/dt;
  // sum
  LFPID = (Kp * LFerror) + (Ki * LFi) + (Kd * LBd);
  LBPID = (Kp * LBerror) + (Ki * LBi) + (Kd * LBd);
  RFPID = (Kp * RFerror) + (Ki * RFi) + (Kd * RFd);
  RBPID = (Kp * RBerror) + (Ki * RBi) + (Kd * RBd);
  // bounds check
  LFPID = constrain(LFPID, -maxPID, maxPID);
  LBPID = constrain(LBPID, -maxPID, maxPID);
  RFPID = constrain(RFPID, -maxPID, maxPID);
  RBPID = constrain(RBPID, -maxPID, maxPID);

  if(targetLFvel == 90)
    LFPID = 0;
  if(targetLBvel == 90)
    LBPID = 0;
  if(targetRFvel == 90)
    RFPID = 0;
  if(targetRBvel == 90)
    RBPID = 0;


  //store info for next time
  LastPIDUpdate = micros();
  lastLFerror = LFerror;
  lastLBerror = LBerror;
  lastRFerror = RFerror;
  lastRBerror = RBerror;
}
//***********************************************************************************

//***********************************************************************************
// Write Speeds to Motors
void setSpeeds()
{
  // output speeds
  motorLF.write(targetLFvel+LFPID);  
  motorLB.write(targetLBvel+LBPID);
  motorRF.write(targetRFvel+RFPID);
  motorRB.write(targetRBvel+RBPID);
  //  motorLF.write(targetLFvel);  
  //  motorLB.write(targetLBvel);
  //  motorRF.write(targetRFvel);
  //  motorRB.write(targetRBvel);
}
//***********************************************************************************


//***********************************************************************************
// Actuates the Kicker
unsigned long kickTimeStamp = 0;

void setKick()
{
  if(kick == 1 && millis() - kickTimeStamp > 1000)
  {
    kickTimeStamp = millis();
    digitalWrite(kickPin, HIGH);    

  }
  else if (millis() - kickTimeStamp > 50)
  {
    digitalWrite(kickPin, LOW);
  }
}
//***********************************************************************************

//***********************************************************************************
// Writes the same value to all motors
void writeAll(int val)
{
  motorLF.write(val);
  motorLB.write(val);
  motorRF.write(val);
  motorRB.write(val);
}
//***********************************************************************************

//***********************************************************************************
void CalibrateESC()
{
  //This only needs to be done if the ESC
  //Needs to be calibrated
  delay(500);
  writeAll(180);  //Set 180 as full throttle  
  delay(4000);
  writeAll(0);    //Set 0 as full reverse
  delay(4000);
  writeAll(90);   //Set 90 as neutral
  delay(4000);  
}
//***********************************************************************************

void printVels()
{
  Serial.print(LFvel);
  Serial.print('\t');
  Serial.print(LBvel);
  Serial.print('\t');
  Serial.print(RFvel);
  Serial.print('\t');
  Serial.print(RBvel);
  Serial.print('\n');
}
//***********************************************************************************
char state = 't';

double targetLFvelSerial = 90;
double targetRFvelSerial = 90;
double targetLBvelSerial = 90;
double targetRBvelSerial = 90;

int kickSerial, chipSerial, dribbleSerial = 0;

void runComm()
{
  if(Serial.available()>=10)
  {
    switch(state)
    {
    case 't':                          // Wait for Start Marker
      if(char(Serial.read()) == char(250))
      {
        state = 'i';
        id = 90; 
        kickSerial = 0;
        //Serial.println("Tilde Received");        
      }         
      break;
    case 'i':                          // Check ID
      id = (int)Serial.read();
      if (id == myid){
        state = 'b';
        //Serial.println("ID Match");        
        break;
      }
      state = 't';
      //Serial.println("ID Incorrect"); 
      break;
    case 'b':                         // Read Packet
      if (Serial.available()>=8)
      {
        //Serial.println("Reading Commands");
        targetLFvelSerial = 90-((int)Serial.read()-100);
        targetLBvelSerial = 90-((int)Serial.read()-100);
        targetRFvelSerial = 90-((int)Serial.read()-100);
        targetRBvelSerial = 90-((int)Serial.read()-100);
        kickSerial = (int)Serial.read();
        chipSerial = (int)Serial.read();
        dribbleSerial = (int)Serial.read();

        if (char(Serial.read()) == char(255))          // Check for End Marker
        {
          state = 't';
          targetLFvel = targetLFvelSerial;
          targetLBvel = targetLBvelSerial;
          targetRFvel = targetRFvelSerial;
          targetRBvel = targetRBvelSerial;
          kick = kickSerial;
          Kp = chipSerial;
          Ki = dribbleSerial;          
          //Serial.println("Packet Complete"); 
        }
        else
        {
          //Serial.println("Packet Incomplete");          
          state = 't';
        }
        break;
      }
      state = 't';    
    } 
  }
}






























