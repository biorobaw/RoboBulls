#include <Servo.h>

// Variables for the communication process
int id;
int myid = 6;    // This Robot's ID
int steerLF = 0; // Global variables for steering.
int steerLB = 0;
int steerRF = 0;
int steerRB = 0;
int kick = 0;

// Motor objects to send the commands to
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

void setup()
{
  //Serial.begin(57600);     // opens serial port, sets data rate to 57600 bps
  
  // Motor set up - the numbers here indicate the pin numbers in the arduino
  // Motor control lines should be connected to these pins
  motor1.attach(11);
  motor2.attach(10);
  motor3.attach(9);
  motor4.attach(8);
  
  // Set the pwms to 0 throttle (which corresponds to 90 in our controllers)
  // This initial signal is needed to start the controller in normal mode (as opposed to program mode)
  motor1.write(90); 
  motor2.write(90);
  motor3.write(90); 
  motor4.write(90);
  
  // wait for the controllers to start - some beeps should be there
  delay(3000);
  
  // After initialization, test the motors all at once. 180 corresponds to full throttle, though the real value was
  // calculated as 135 initially (90 + 35), and full backwards 45 (90-45)
  motor1.write(180);  
  motor2.write(180);
  motor3.write(180); 
  motor4.write(180);   
  
  delay(5000);

  // Stop the motors by sending 0 throttle (90 pwm)
  motor1.write(90);  
  motor2.write(90);
  motor3.write(90); 
  motor4.write(90);
}

char state = 't';

void loop()
{

  if (Serial.available()>0)
  {
    switch(state)
    {
    case 't':                          // Wait for Tilde
      if(Serial.read() == '~')
        state = 'i';
      break;
  
    case 'i':                          // Check ID
      id = Serial.read();
      if (id == myid){
        state = 'b';
        break;
      }
      state = 't'; 
      break;
  
    case 'b':                         // Read Packet
      if (Serial.available()>=8)
      {
          steerLF = Serial.read();
          steerLB = Serial.read();
          steerRF = Serial.read();
          steerRB = Serial.read();
          kick = Serial.read();
          Serial.read();
          Serial.read();
      
          if (Serial.read()=='$')          // Check for Dollar
          {
            state = 's';
          }
          else
          {
            id = steerLF = steerLB = steerRF = steerRB = kick = 0;
            state = 't';
          }
          break;
      }
      state = 't';
      break; 
  
    case 's':                         
      motor1.write(steerLF*1.8);  
      motor2.write(steerLB*1.8);
      motor3.write(steerRF*1.8);
      motor4.write(steerRB*1.8);
      state = 't';  
    }  
  }
}

// Full reverse, 1ms pulse, 0.66v. Arduino - write 45, 1v
// Full Forward, 2ms pulse, 1.17v, Arduino - write 140, 1.955v
// Stop 1.5ms pulse, 1v. Arduino - write  90, 1.5v

// Packet Format:
// (~)(1)(100)(100)(100)(100)(k)(0)(0)($)


//  www.github.com  mllofriu/robobulls2

//  robobulls2 / robotc / RBKicker.c 

//  robobulls2 / communication / nxtrobcomm.cpp 

