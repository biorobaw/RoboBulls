import processing.serial.*;

Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph 

//Variables to draw a continuous line.
int lastxPos=1;
int lastheight=0;

void setup () {
  // set the window size:
  size(1200, 360);        

  // List all the available serial ports
  println(Serial.list());
  // Check the listed serial ports in your machine
  // and use the correct index number in Serial.list()[].

  myPort = new Serial(this, Serial.list()[0], 115200);  //

  // A serialEvent() is generated when a newline character is received :
  myPort.bufferUntil('\n');
  background(0);      // set inital background:
}
void draw () {
  // everything happens in the serialEvent()
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);                // trim off whitespaces.
    float inByte = float(inString);           // convert to a number.
    inByte = map(inByte, 0, 360, 0, height); //map to the screen height.

    //Drawing a line from Last inByte to the new one.
    stroke(255, 0, 0);     //stroke color
    strokeWeight(2);        //stroke wider
    line(lastxPos, lastheight, xPos, height-inByte*2); 
    lastxPos = xPos;
    lastheight = int(height-inByte*2);

    // at the edge of the window, go back to the beginning:
    if (xPos >= width) {
      xPos = 0;
      lastxPos= 0;
      background(0);  //Clear the screen.
      
      //Draw markers
      stroke(255, 255, 255);     //stroke color
      strokeWeight(1);        //stroke wider
      for (int i = 0; i <=height; i+=40)
      {
        line(0, i, width, i);
        line(0, i+20, width, i+20);
        text((height-i)/2, 0, i );
      }
    } else {
      // increment the horizontal position:
      xPos++;
    }
  }
}

