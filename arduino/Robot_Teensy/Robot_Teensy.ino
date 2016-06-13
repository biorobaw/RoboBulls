int id;
int myid = 4;    // This Robot's ID


// Declare motor pins
#define enablePinLF 2
#define dirPinLF 3
#define brakePinLF 4

#define enablePinLB 5
#define dirPinLB 6
#define brakePinLB 7

#define enablePinRF 19
#define dirPinRF 18
#define brakePinRF 17

#define enablePinRB 16
#define dirPinRB 15
#define brakePinRB 14

#define speedPinLF 23
#define speedPinLB 22
#define speedPinRF 21
#define speedPinRB 20

// Define pin locations for kicker and chipper
#define dribblePin 10
#define kickPin 9
#define chargePin 8

// Define pin locations for reading capacitor
#define readCapPin A10
#define read250Pin A11
#define ledPin 13

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

  pinMode(ledPin, OUTPUT);
 

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
  //blinkReadCap();
  //printVels();
}
//***********************************************************************************

//***********************************************************************************
// Actuates the Kicker
unsigned long kickStartTime = 0;
unsigned long chargeStartTime = 0;
unsigned long chargeTime = 6000; //ms
unsigned long kickTime = 15;  //ms
enum kickerState {
  kicking, charging
};
kickerState current = charging;
void setKick()
{
  switch (current)
  {
    case charging:
      digitalWrite(kickPin, LOW);
      digitalWrite(chargePin, HIGH);
      if (kick == 'k' && millis() - chargeStartTime >= chargeTime)
      {
        current = kicking;
        kickStartTime = millis();
      }
      break;
    case kicking:
      digitalWrite(kickPin, HIGH);
      digitalWrite(chargePin, LOW);
      if (millis() - kickStartTime >= kickTime)
      {
        digitalWrite(kickPin, LOW);
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
    digitalWrite(dribblePin, HIGH);
  else  
    digitalWrite(dribblePin, LOW);    
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

//Blink Reading**********************************************************************
unsigned long last_blink_time = 0;
void blinkReadCap()
{
  unsigned int pin_val = analogRead(readCapPin);  
  
  // High Voltage -> High Frequency Blink
  float blink_delay = map(pin_val, 0, 1023, 2000, 0);

  if(millis() - last_blink_time > blink_delay)  
  {
    digitalWrite(ledPin, HIGH);
    last_blink_time = millis();
  }
  else
    digitalWrite(ledPin, LOW );      
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
          //Serial.println("Tilde Received");
        }
        break;
      case 'i':                          // Check ID
        id = (int)Serial1.read();
        if (id == myid) {
          state = 'b';          
          //Serial.println("ID Match");
          break;
        }
        state = 't';
        //Serial.println("ID Incorrect");
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






