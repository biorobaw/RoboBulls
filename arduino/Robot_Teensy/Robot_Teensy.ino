#include <Encoder.h>
#include <Servo.h>
int id;
int myid = 4;    // This Robot's ID

// Declare motor pins
//#define gndPinLF 
#define enablePinLF 2
#define dirPinLF 3
#define brakePinLF 4

#define gndPinLB 5
#define enablePinLB 6
#define dirPinLB 7
#define brakePinLB 8

#define gndPinRF 9
#define enablePinRF 10
#define dirPinRF 11
#define brakePinRF 12

#define gndPinRB 13
#define enablePinRB 14
#define dirPinRB 15
#define brakePinRB 16

#define speedPinLF 20
#define speedPinLB 21
#define speedPinRF 22
#define speedPinRB 23

// Define pin locations for kicker and chipper
#define dribblePin 17
#define kickPin 18
#define chargePin 19


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


//***********************************************************************************
void setup()
{
  // Set Kicker and Chipper pins to output
  pinMode(kickPin, OUTPUT);
  pinMode(chargePin, OUTPUT);
  pinMode(dribblePin, OUTPUT);

  // Set controller signal pins to output
  //pinMode(gndPinLF, OUTPUT);
  pinMode(gndPinLB, OUTPUT);
  pinMode(gndPinRF, OUTPUT);
  pinMode(gndPinRB, OUTPUT);

  pinMode(enablePinLF, OUTPUT);
  pinMode(enablePinLB, OUTPUT);
  pinMode(enablePinRF, OUTPUT);
  pinMode(enablePinRB, OUTPUT);

  pinMode(dirPinLF, OUTPUT);
  pinMode(dirPinLB, OUTPUT);
  pinMode(dirPinRF, OUTPUT);
  pinMode(dirPinRB, OUTPUT);

  pinMode(brakePinLF, OUTPUT);
  pinMode(brakePinLB, OUTPUT);
  pinMode(brakePinRF, OUTPUT);
  pinMode(brakePinRB, OUTPUT);

  // Set Controller Signal GND to 0V
  //digitalWrite(gndPinLF, LOW);
  digitalWrite(gndPinLB, LOW);
  digitalWrite(gndPinRF, LOW);
  digitalWrite(gndPinRB, LOW);

  // Enable Controllers
  digitalWrite(enablePinLF, HIGH);
  digitalWrite(enablePinLB, HIGH);
  digitalWrite(enablePinRF, HIGH);
  digitalWrite(enablePinRB, HIGH);

  // Enable Brakes
  digitalWrite(brakePinLF, HIGH);
  digitalWrite(brakePinLB, HIGH);
  digitalWrite(brakePinRF, HIGH);
  digitalWrite(brakePinRB, HIGH);

  // Set Speed to Zero
  analogWrite(speedPinLF, 30);
  analogWrite(speedPinLB, 30);
  analogWrite(speedPinRF, 30);
  analogWrite(speedPinRB, 30);

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
  if (millis() - LastLoopUpdate > 10)
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
int kickTime = 15;  //ms
enum kickerState {
  kicking, charging
};
kickerState current = charging;
void setKick()
{
  switch (current)
  {
    case charging:
      digitalWrite(kickPin, HIGH);
      digitalWrite(chargePin, LOW);
      if (kick == 'k' && millis() - chargeStartTime >= chargeTime)
      {
        current = kicking;
        kickStartTime = millis();
      }
      break;
    case kicking:
      digitalWrite(kickPin, LOW);
      digitalWrite(chargePin, HIGH);
      if (millis() - kickStartTime >= kickTime)
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

void setDribble()
{
  if(dribble=='d')
    digitalWrite(dribblePin, LOW);
  else  
    digitalWrite(dribblePin, HIGH);    
}
//***********************************************************************************

//***********************************************************************************
// Write Speeds to Motors
void setSpeeds()
{
  //Bound for PWM duty cycles, about 10% and 90% of 0 and 255
  static int lowPWM = 30, highPWM = 225;

//  targetLFvel = targetLBvel = targetRFvel = targetRBvel = 20;

  // Output speeds
  if (targetLFvel > 0 )
  {
    analogWrite(speedPinLF, map(abs(targetLFvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinLF, HIGH);
    digitalWrite(brakePinLF, LOW);
  }
  else if (targetLFvel < 0 )
  {
    analogWrite(speedPinLF, map(abs(targetLFvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinLF, LOW);
    digitalWrite(brakePinLF, LOW);
  }
  else
    digitalWrite(brakePinLF, HIGH);

  if (targetLBvel > 0 )
  {
    analogWrite(speedPinLB, map(abs(targetLBvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinLB, HIGH);
    digitalWrite(brakePinLB, LOW);
  }
  else if (targetLBvel < 0)
  {
    analogWrite(speedPinLB, map(abs(targetLBvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinLB, LOW);
    digitalWrite(brakePinLB, LOW);
  }
  else
    digitalWrite(brakePinLB, HIGH);

  if (targetRFvel > 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinRF, LOW);
    digitalWrite(brakePinRF, LOW);
  }
  else if (targetRFvel < 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinRF, HIGH);
    digitalWrite(brakePinRF, LOW);
  }
  else
    digitalWrite(brakePinRF, HIGH);

  if (targetRBvel > 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinRB, LOW);
    digitalWrite(brakePinRB, LOW);
  }
  else if (targetRBvel < 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel), 0, 100, lowPWM, highPWM));
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
  if (Serial1.available() >= 10)
  {
    switch (state)
    {
      case 't':                          // Wait for Start Marker
        if (char(Serial1.read()) == char(250))
        {
          state = 'i';
          id = 90;
          kickSerial = 0;
          //Serial1.println("Tilde Received");
        }
        break;
      case 'i':                          // Check ID
        id = (int)Serial1.read();
        if (id == myid) {
          state = 'b';
          //        digitalWrite(13, LOW);
          //Serial1.println("ID Match");
          break;
        }
        state = 't';
        //Serial1.println("ID Incorrect");
        break;
      case 'b':                         // Read Packet
        if (Serial1.available() >= 8)
        {
          //Serial1.println("Reading Commands");
          targetLFvelSerial = ((int)Serial1.read() - 100);
          targetLBvelSerial = ((int)Serial1.read() - 100);
          targetRFvelSerial = (int)Serial1.read() - 100;
          targetRBvelSerial = ((int)Serial1.read() - 100);
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
            dribble = dribbleSerial;
            chip = chipSerial;

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






