#include <Encoder.h>
#include <Servo.h>
int id;
int myid = 4;    // This Robot's ID

// Declare motor pins
#define speedPinLF 4
#define gndPinLF 24
#define enablePinLF 25  
#define dirPinLF 26
#define brakePinLF 27

#define speedPinLB 5
#define gndPinLB 32
#define enablePinLB 33
#define dirPinLB 34
#define brakePinLB 35

#define speedPinRF 6 
#define gndPinRF 40
#define enablePinRF 41 
#define dirPinRF 42
#define brakePinRF 43

#define speedPinRB 7
#define gndPinRB 48
#define enablePinRB 49 
#define dirPinRB 50
#define brakePinRB 51

// Define pin locations for kicker and chipper
#define dribblePin 8
#define kickPin 9
#define chargePin 10


// Velocities
//  90 = Stop
// 180 = Full Forward 
//   0 = Full Reverse
double targetLFvel = 0;
double targetRFvel = 0;
double targetLBvel = 0;
double targetRBvel = 0;


// Kicker Chipper Dribbler
int kick, chip, dribble = 0;

// Encoder Variables
Encoder encoder(2, 3);
unsigned long pos;
float averageVel;
unsigned long lastUpdTime;

//***********************************************************************************
void setup()
{   
  // Debug
  //  pinMode(13, OUTPUT);
  //  digitalWrite(13,HIGH);

  // Set Kicker and Chipper pins to output
  pinMode(kickPin, OUTPUT);
  pinMode(chargePin, OUTPUT);
  pinMode(dribblePin, OUTPUT);    

  // Set controller signal pins to output
  pinMode(gndPinLF,OUTPUT);
  pinMode(gndPinLB,OUTPUT);
  pinMode(gndPinRF,OUTPUT);
  pinMode(gndPinRB,OUTPUT);

  pinMode(enablePinLF,OUTPUT);
  pinMode(enablePinLB,OUTPUT);
  pinMode(enablePinRF,OUTPUT);
  pinMode(enablePinRB,OUTPUT); 

  pinMode(dirPinLF,OUTPUT);
  pinMode(dirPinLB,OUTPUT);
  pinMode(dirPinRF,OUTPUT);
  pinMode(dirPinRB,OUTPUT);

  pinMode(brakePinLF,OUTPUT);
  pinMode(brakePinLB,OUTPUT);
  pinMode(brakePinRF,OUTPUT);
  pinMode(brakePinRB,OUTPUT);

  // Set Controller Signal GND to 0V
  digitalWrite(gndPinLF,LOW);
  digitalWrite(gndPinLB,LOW);
  digitalWrite(gndPinRF,LOW);
  digitalWrite(gndPinRB,LOW);

  // Enable Controllers
  digitalWrite(enablePinLF,HIGH);
  digitalWrite(enablePinLB,HIGH);
  digitalWrite(enablePinRF,HIGH);
  digitalWrite(enablePinRB,HIGH);

  // Enable Brakes
  digitalWrite(brakePinLF,HIGH);
  digitalWrite(brakePinLB,HIGH);
  digitalWrite(brakePinRF,HIGH);
  digitalWrite(brakePinRB,HIGH);

  // Set Speed to Zero
  analogWrite(speedPinLF,30);
  analogWrite(speedPinLB,30);
  analogWrite(speedPinRF,30);
  analogWrite(speedPinRB,30);    

  // Encoder Variables
  pos = encoder.read();
  lastUpdTime = millis();
  averageVel = 0;  

  //Note: For the UNO, the USB/Micro
  //switch must be in the micro position. 

  // Start Serial Port
  Serial1.begin(57600);

}
//***********************************************************************************

//***********************************************************************************
long timestamp = 0;
long LastLoopUpdate = 0;
void loop()
{
  runComm();
  if(millis()-LastLoopUpdate > 10)
  {
    setSpeeds();
    setKick();
    setDribble();
    LastLoopUpdate = millis(); 
  }
  //printVels();
}
//***********************************************************************************

//***********************************************************************************
// Actuates the Kicker
unsigned long kickStartTime = 0;
unsigned long chargeStartTime = 0;
int chargeTime = 6000; //ms
int kickTime = 30;  //ms
enum kickerState {
  kicking, charging};
kickerState current = charging;
void setKick()
{
  switch(current)
  {
  case charging:
    digitalWrite(kickPin, HIGH);
    digitalWrite(chargePin, LOW);
    if(kick == 'k' && millis()-chargeStartTime >= chargeTime)
    {
      current = kicking;
      kickStartTime = millis();
    }
    break;
  case kicking:
    digitalWrite(kickPin, LOW);
    digitalWrite(chargePin, HIGH);  
    if(millis()-kickStartTime >= kickTime)
    {
      digitalWrite(kickPin, HIGH);
      current = charging;
      chargeStartTime = millis();
    }
  }
}
//***********************************************************************************

//***********************************************************************************
// Manages the dribbler

const float alpha = 1;
const int TICKS_PER_TURN = 12 * 30;
const float TICKS_MILLI_2_TURNS_SEC = 1000 / TICKS_PER_TURN;

unsigned long dribble_start_time = 0;
unsigned long recovery_start_time = 0;
char dribbler_state = 'i';
/*
 i = initial
 w = wait for start
 c = check for min speed
 r = recovery
 */

void setDribble()
{
  unsigned long newPos, newTime;
  newPos = encoder.read();
  newTime = millis();

  if (newTime > lastUpdTime){
    averageVel = (1-alpha) * averageVel + alpha * ((float)(newPos-pos))/(newTime-lastUpdTime) * TICKS_MILLI_2_TURNS_SEC;
    pos = newPos;
    lastUpdTime = newTime;
  }
  
  Serial1.println(averageVel);  
  if(dribble == 1)
  {
    // State Machine
    if(dribbler_state == 'i')  // initial
    {
      // Attempt to start the dribbler
      digitalWrite(dribblePin, LOW);     //Active Low
      dribble_start_time = millis();
      dribbler_state = 'w';
    }

    else if( dribbler_state == 'w')  // wait for start
    {
      // Wait for the dribbler to rev up
      if  ( millis()-dribble_start_time > 1500)
        dribbler_state = 'c';
    }

    else if( dribbler_state == 'c') // check for min speed
    {
      if(averageVel < 0.5)  // Too Slow
      {
        digitalWrite(dribblePin, HIGH);
        recovery_start_time = millis();
        dribbler_state = 'r';
      }
    }

    else if( dribbler_state == 'r') // recovery
    {
      if( millis() - recovery_start_time >  3000)
        dribbler_state = 'i';
    }
  }
  else
  {
    digitalWrite(dribblePin, HIGH);
    dribbler_state = 'i';  // reset dribbler state machine
  }
}
//***********************************************************************************

//***********************************************************************************
// Write Speeds to Motors
void setSpeeds()
{
  //Bound for PWM duty cycles, about 10% and 90% of 0 and 255
  static int lowPWM = 30, highPWM = 225;
  
  // Output speeds
  if(targetLFvel > 0 )
  {
    analogWrite(speedPinLF, map(abs(targetLFvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinLF, LOW);
    digitalWrite(brakePinLF, LOW);
  }
  else if(targetLFvel < 0 )
  {
    analogWrite(speedPinLF, map(abs(targetLFvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinLF, HIGH);
    digitalWrite(brakePinLF, LOW);
  }
  else
    digitalWrite(brakePinLF, HIGH);

  if(targetLBvel > 0 )
  {
    analogWrite(speedPinLB, map(abs(targetLBvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinLB, HIGH);
    digitalWrite(brakePinLB, LOW);
  }
  else if(targetLBvel < 0)
  {
    analogWrite(speedPinLB, map(abs(targetLBvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinLB, LOW);
    digitalWrite(brakePinLB, LOW);
  }
  else
    digitalWrite(brakePinLB, HIGH);

  if(targetRFvel > 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinRF, LOW);
    digitalWrite(brakePinRF, LOW);
  }
  else if(targetRFvel < 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinRF, HIGH);
    digitalWrite(brakePinRF, LOW);
  }
  else
    digitalWrite(brakePinRF, HIGH);

  if(targetRBvel > 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinRB, LOW);
    digitalWrite(brakePinRB, LOW);
  }
  else if(targetRBvel < 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel),0,100,lowPWM,highPWM));
    digitalWrite(dirPinRB, HIGH);
    digitalWrite(brakePinRB, LOW);
  }
  else
    digitalWrite(brakePinRB, HIGH);
}
//***********************************************************************************

//***********************************************************************************

void printVels()
{
  Serial1.print(targetLFvel);
  Serial1.print('\t');
  Serial1.print(targetLBvel);
  Serial1.print('\t');
  Serial1.print(targetRFvel);
  Serial1.print('\t');
  Serial1.print(targetRBvel);
  Serial1.print('\n');
}
//***********************************************************************************
char state = 't';

double targetLFvelSerial = 0;
double targetRFvelSerial = 0;
double targetLBvelSerial = 0;
double targetRBvelSerial = 0;

int kickSerial, chipSerial, dribbleSerial = 0;

void runComm()
{
  if(Serial1.available()>=10)
  {
    switch(state)
    {
    case 't':                          // Wait for Start Marker
      if(char(Serial1.read()) == char(250))
      {
        state = 'i';
        id = 90; 
        kickSerial = 0;
        //Serial1.println("Tilde Received");        
      }         
      break;
    case 'i':                          // Check ID
      id = (int)Serial1.read();
      if (id == myid){
        state = 'b';
        //        digitalWrite(13, LOW);
        //Serial1.println("ID Match");        
        break;
      }
      state = 't';
      //Serial1.println("ID Incorrect"); 
      break;
    case 'b':                         // Read Packet
      if (Serial1.available()>=8)
      {
        //Serial1.println("Reading Commands");
        targetLFvelSerial = ((int)Serial1.read()-100);
        targetLBvelSerial = ((int)Serial1.read()-100);
        targetRFvelSerial = (int)Serial1.read()-100;
        targetRBvelSerial = ((int)Serial1.read()-100);
        kickSerial = (int)Serial1.read();
        chipSerial = (int)Serial1.read();
        dribbleSerial = (int)Serial1.read();

        if (char(Serial1.read()) == char(255))          // Check for End Marker
        {
          state = 't';
          targetLFvel = targetLFvelSerial;
          targetLBvel = targetLBvelSerial;
          targetRFvel = targetRFvelSerial;
          targetRBvel = targetRBvelSerial;
          kick = kickSerial;

          //Serial1.println("Packet Complete"); 
        }
        else
        {
          //Serial1.println("Packet Incomplete");          
          state = 't';
        }
        break;
      }
      state = 't';    
    } 
  }
}






