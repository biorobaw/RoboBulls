/****************************************\
 * PID Feedback Motor Control
 * R. Mattes
 * Penn State Robotics Club
 * www.psurobotics.org
 \****************************************/

#include <Encoder.h>
#include <Servo.h>

#define ENCODER_OPTIMIZE_INTERRUPTS

int id;
int myid = 6;    // This Robot's ID

// Define pin locations and delay time between each query for wheel encoding
#define LFEncPinA 2
#define LFEncPinB A0
#define LBEncPinA 3
#define LBEncPinB A1
#define RFEncPinA A2
#define RFEncPinB A3
#define RBEncPinA A4
#define RBEncPinB A5

// Define an approximate constant for conversion between encoder ticks and speed
double Kvel = 0.05;
int maxPID = 90;

// PID Gains
double Kp = 1;
double Ki = 0.0;
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

//***********************************************************************************
void setup()
{

  //Start Serial Port
  Serial.begin(115200);
  
  //Note: For the UNO, the USB/Micro
  //switch must be in the micro position. 
}
//***********************************************************************************

//***********************************************************************************
char state = 't';

void loop()
{
  
  // Calculate current motor speed
  GetSpeeds();
  
  Serial.println(LFvel);
}
//***********************************************************************************


//***********************************************************************************
// Return speed (scaled using kvel)
// Define Encoders
Encoder LFEnc(LFEncPinA, LFEncPinB);
Encoder LBEnc(LBEncPinA, LBEncPinB);
Encoder RFEnc(RFEncPinA, RFEncPinB);
Encoder RBEnc(RBEncPinA, RBEncPinB);
long LastSpeedUpdate = 0;

void GetSpeeds()
{
  // Update Encoder Position
  LFEncPos = LFEnc.read();
  LBEncPos = LBEnc.read();
  RFEncPos = RFEnc.read();
  RBEncPos = RBEnc.read();

  if(millis()-LastSpeedUpdate > 5)
  {
    // If an integer overflow occurs, throw out the new value
    // Otherwise, scale the difference to get speed
    if(abs(LFEncPos - LFEncPosPrev) < 60000)
      LFvel = 90-Kvel*(LFEncPos - LFEncPosPrev);
    if(abs(LBEncPos - LBEncPosPrev) < 60000)
      LBvel = 90-Kvel*(LBEncPos - LBEncPosPrev);
    if(abs(RFEncPos - RFEncPosPrev) < 60000)
      RFvel = 90+Kvel*(RFEncPos - RFEncPosPrev);
    if(abs(LBEncPos - LBEncPosPrev) < 60000)
      RBvel = 90+Kvel*(RBEncPos - RBEncPosPrev);

    // Store info for next time
    LFEncPosPrev = LFEncPos;
    LBEncPosPrev = LBEncPos;
    RFEncPosPrev = RFEncPos;
    RBEncPosPrev = RBEncPos;    
    LastSpeedUpdate = millis(); 
  }
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

void SetSpeeds()
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
  RFPID = constrain(RFPID, -maxPID, maxPID);
  RFPID = constrain(RFPID, -maxPID, maxPID);
  RFPID = constrain(RFPID, -maxPID, maxPID);
  // output speeds
  motorLF.write(90-LFPID);  
  motorLB.write(90-LBPID);
  motorRF.write(90+RFPID);
  motorRB.write(90+RBPID);
//  motorLF.write(targetLFvel);  
//  motorLB.write(targetLBvel);
//  motorRF.write(targetRFvel);
//  motorRB.write(targetRBvel);
  // store info for next time
  LastPIDUpdate = micros();
  lastLFerror = LFerror;
  lastLBerror = LBerror;
  lastRFerror = RFerror;
  lastRBerror = RBerror;
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
  writeAll(180);  //Set 180 as full throttle  
  delay(4000);
  writeAll(0);    //Set 0 as full reverse
  delay(4000);
  writeAll(90);   //Set 90 as neutral
  delay(4000);  
}
//***********************************************************************************








