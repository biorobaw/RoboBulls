/****************************************\
 * PID Feedback Motor Control
 * R. Mattes
 * Penn State Robotics Club
 * www.psurobotics.org
 * Modified for Robobulls by Muhaimen Shamsi
 * Modified from Shamsi by Fallon, James, et al
 \****************************************/
/* CHANGELOG
 * 8/11/15: James: Changed ID from 2 to 4
 * 8/5/15 : James: Swapped all RF and RB pins, as they were backwards. Note the wiring on the robot is now labeled wrong
 */

int myid = 4;    // This Robot's ID

// Declare motor pins
#define enablePinLF 44
#define enablePinLB 11
#define enablePinRF 47
#define enablePinRB 27

#define speedPinLF 6
#define speedPinLB 10
#define speedPinRF 3
#define speedPinRB 5

#define dirPinLF 7
#define dirPinLB 13 
#define dirPinRF 51
#define dirPinRB 23

#define brakePinLF 22
#define brakePinLB 12
#define brakePinRF 49
#define brakePinRB 25

// Define pin locations for kicker and chipper
#define kickPin 8

// Velocities
//  90 = Stop
// 180 = Full Forward 
//   0 = Full Reverse
double targetLFvel = 0;
double targetRFvel = 0;
double targetLBvel = 0;
double targetRBvel = 0;

double LFvel = 0;
double RFvel = 0;
double LBvel = 0;
double RBvel = 0;

// Kicker Chipper Dribbler
int kick, chip, dribble = 0;

//***********************************************************************************
void setup()
{   
  // Set Kicker and Chipper pins to output
  pinMode(kickPin, OUTPUT);

  // Set Motor Pins to output
  pinMode(enablePinLF,OUTPUT);
  pinMode(enablePinLB,OUTPUT);
  pinMode(enablePinRF,OUTPUT);
  pinMode(enablePinRB,OUTPUT);
  
  digitalWrite(enablePinLF,HIGH);
  digitalWrite(enablePinLB,HIGH);
  digitalWrite(enablePinRF,HIGH);
  digitalWrite(enablePinRB,HIGH);
  
  analogWrite(speedPinLF,30);
  analogWrite(speedPinLB,30);
  analogWrite(speedPinRF,30);
  analogWrite(speedPinRB,30);
    
  pinMode(dirPinLF,OUTPUT);
  pinMode(dirPinLB,OUTPUT);
  pinMode(dirPinRF,OUTPUT);
  pinMode(dirPinRB,OUTPUT);
  
  pinMode(brakePinLF,OUTPUT);
  pinMode(brakePinLB,OUTPUT);
  pinMode(brakePinRF,OUTPUT);
  pinMode(brakePinRB,OUTPUT);

  //Note: For the UNO, the USB/Micro
  //switch must be in the micro position. 

  // Start Serial Port
  Serial.begin(57600);
  
}

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
    LastLoopUpdate = millis(); 
  }
}

//***********************************************************************************
// Write Speeds to Motors
void setSpeeds()
{
  // output speeds
  if(targetLFvel > 0 )
  {
    analogWrite(speedPinLF, map(abs(targetLFvel),0,100,25,230));
    digitalWrite(dirPinLF, LOW);
    digitalWrite(brakePinLF, LOW);
  }
  else if(targetLFvel < 0 )
  {
    analogWrite(speedPinLF, map(abs(targetLFvel),0,100,25,230));
    digitalWrite(dirPinLF, HIGH);
    digitalWrite(brakePinLF, LOW);
  }
  else
    digitalWrite(brakePinLF, HIGH);
    
      
  if(targetLBvel > 0 )
  {
    analogWrite(speedPinLB, map(abs(targetLBvel),0,100,25,230));
    digitalWrite(dirPinLB, LOW);
    digitalWrite(brakePinLB, LOW);
  }
  else if(targetLBvel < 0)
  {
    analogWrite(speedPinLB, map(abs(targetLBvel),0,100,25,230));
    digitalWrite(dirPinLB, HIGH);
    digitalWrite(brakePinLB, LOW);
  }
  else
    digitalWrite(brakePinLB, HIGH);
    
  if(targetRFvel > 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel),0,100,25,230));
    digitalWrite(dirPinRF, LOW);
    digitalWrite(brakePinRF, LOW);
  }
  else if(targetRFvel < 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel),0,100,25,230));
    digitalWrite(dirPinRF, HIGH);
    digitalWrite(brakePinRF, LOW);
  }
  else
    digitalWrite(brakePinRF, HIGH);
    
  if(targetRBvel > 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel),0,100,25,230));
    digitalWrite(dirPinRB, LOW);
    digitalWrite(brakePinRB, LOW);
  }
  else if(targetRBvel < 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel),0,100,25,230));
    digitalWrite(dirPinRB, HIGH);
    digitalWrite(brakePinRB, LOW);
  }
  else
    digitalWrite(brakePinRB, HIGH);
    
}

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
        kickSerial = 0;
        //Serial.println("Tilde Received");        
      }         
      break;
    case 'i':                          // Check ID
      if ((int)Serial.read() == myid){
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
        targetLFvelSerial = (int)Serial.read()-100;
        targetLBvelSerial = (int)Serial.read()-100;
        targetRFvelSerial = (int)Serial.read()-100;
        targetRBvelSerial = (int)Serial.read()-100;
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

