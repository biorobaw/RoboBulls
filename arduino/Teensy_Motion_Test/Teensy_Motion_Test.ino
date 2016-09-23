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

// Velocities Range = {-100, 100}
double targetLFvel = 25;
double targetRFvel = 25;
double targetLBvel = 25;
double targetRBvel = 25;

//***********************************************************************************
void setup()
{
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
}
//***********************************************************************************

//***********************************************************************************

void loop()
{
   setSpeeds();

   // Reverse direction every 2000 seconds   
   if(millis()%4000 < 2000)
   {
      targetLFvel *= -1;
      targetRFvel *= -1;
      targetLBvel *= -1;
      targetRBvel *= -1;
   }
    
}
//***********************************************************************************


//***********************************************************************************
// Write Speeds to Motors
void setSpeeds()
{
  // Bounds for PWM duty cycles, about 10% and 90% of 0 and 255
  static int lowPWM = 35, highPWM = 225;

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
    digitalWrite(dirPinRF, HIGH);
    digitalWrite(brakePinRF, LOW);
  }
  else if (targetRFvel < 0 )
  {
    analogWrite(speedPinRF, map(abs(targetRFvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinRF, LOW);
    digitalWrite(brakePinRF, LOW);
  }
  else
    digitalWrite(brakePinRF, HIGH);

  if (targetRBvel > 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinRB, HIGH);
    digitalWrite(brakePinRB, LOW);
  }
  else if (targetRBvel < 0 )
  {
    analogWrite(speedPinRB, map(abs(targetRBvel), 0, 100, lowPWM, highPWM));
    digitalWrite(dirPinRB, LOW);
    digitalWrite(brakePinRB, LOW);
  }
  else
    digitalWrite(brakePinRB, HIGH);
}
//***********************************************************************************







