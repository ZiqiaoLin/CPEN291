/* Author: Yuxiang Huang
 * Date: February 3th, 2017
 * Purpose: To get a better understanding of designing 
 * a user interface and Processing; finsih CPEN 291
 *Lab 4
*/ 

import controlP5.*;
import processing.serial.*;
Serial myPort;
//This is an external libray for button controlling
ControlP5 gui;

int start;

/*********************************************************************/
int numValues = 4; // number of input values or sensors
// * change this to match how many values your Arduino is sending *

//Create arrays to store data received from the serial port,
//As well well as some other parameters such as colors 
float[] values = new float[numValues];
int[] min = new int[numValues];
int[] max = new int[numValues];
color[] valColor = new color[numValues];

//Define the height of the title
int titleH = 100; //title height
float partH; // partial screen height

float xPos = 0; // horizontal position of the graph
boolean clearScreen = true; // flagged when graph has filled screen

void setup() {
  //Define the size of the window
  size(1000, 700);
  gui = new ControlP5(this);
  gui.addButton("Start")
     .setPosition(870, 240)
     .setSize(80,80)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
     
  gui.addButton("Stop")
     .setPosition(870, 400)
     .setSize(80,80)
     .setValue(0)
     .activateBy(ControlP5.RELEASE);
  
  partH = (float)(height - titleH) / numValues;

  // List all the available serial ports:
  printArray(Serial.list());
  // First port [0] in serial list is usually Arduino, but *check every time*:
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  // don't generate a serialEvent() until getting a newline character:
  myPort.bufferUntil('\n');

  textSize(12);

  background(0);
  noStroke();

  // initialize:
  values[0] = 0;
  min[0] = 0;
  max[0] = 50; // full range example, e.g. any analogRead
  valColor[0] = color(255, 0, 0); // red

  values[1] = 0; 
  min[1] = 0;
  max[1] = 30;  // partial range example, e.g. IR distance sensor
  valColor[1] = color(0, 255, 0); // green

  values[2] = 0;
  min[2] = 0;
  max[2] = 300;    // digital input example, e.g. a button
  valColor[2] = color(0, 0, 255); // blue
  
 // example for adding a 4th value:
  values[3] = 0;
  min[3] = 0;
  max[3] = 1040; // custom range example 
  valColor[3] = color(255, 0, 255); // purple
   
   /*values[4] = 0;
   min[4] = 0;
   max[4] = 1023; // custom range example 
   valColor[4] = color(255, 0, 255); // purple*/
}


void draw() {
  //Set the backgound color (black) when the draw function is first executed
      if (clearScreen) {
        // erase screen with black:
        background(0); 
    
        // or, erase screen with translucent black:
        //fill(0,200);
        //noStroke();
        //rect(0,0,width - 200,height);
        clearScreen = false; // reset flag
      } 
      
      //Set up the title
      textSize(50);
      fill(102,204,255);
      text("CPEN 291 Lab4 User Interface", 140, 60);
      textSize(12);
    
    //If the start button is pushed, start to plot the graph
    if(start == 1){
      for (int i=0; i<numValues; i++) {
    
        // map to the range of partial screen height:
        float mappedVal = map(values[i], min[i], max[i], 0, partH);
    
        // draw plot lines:
        stroke(valColor[i]);
        line(xPos, partH*(i+1) - mappedVal + titleH, xPos, partH*(i+1) - mappedVal + 1 + titleH);
    
        // draw dividing line:
        stroke(255);
        line(0, height - partH*(i+1), width - 200, height - partH*(i+1));
        line(0, titleH, width, titleH);
        stroke(255);
        line(800, titleH, 800, height);
    
        // display values on screen:
        fill(50);
        noStroke();
        rect(0, partH*i+1+titleH, 230, 15);
        fill(255);
        
        //Draw additional dividing lines according to the value of i
        //(which indicates which element it is plotting
        switch(i){
          case (0): text("Temperature ( *C ):", 2, partH*0+12+titleH);
          case (1): text("Humidity ( % ):", 2, partH*1+12+titleH);
          case (2): text("Temperature (LM35)(*C):", 2, partH*2+12+titleH);
          case (3): text("Relative Light Level: ", 2, partH*3+12+titleH);
        }
        
        text(round(values[i]), 160, partH*i+12+titleH);
        fill(125);
        text(max[i], 200, partH*i+12+titleH);
    
        //print(i + ": " + values[i] + "\t"); // Used for debugging
        //println("\t"+mappedVal); //Used for debugging
      }
      
      // increment the graph's horizontal position:
      xPos = xPos + 0.5;
      // if at the edge of the screen, go back to the beginning:
      if (xPos > width - 200) {
        xPos = 0;
        clearScreen = true;
      }
    } 
}


void serialEvent(Serial myPort) { 
    try {
      // get the ASCII string:
      String inString = myPort.readStringUntil('\n');
      //println("raw: \t" + inString); // Used for debugging
  
      if (inString != null) {
        // trim off any whitespace:
        inString = trim(inString);
  
        // split the string on the delimiters and convert the resulting substrings into an float array:
        values = float(splitTokens(inString, " ")); // delimiter can be comma space or tab
        for(int i = 0; i < numValues; i++){
          println(values[i]); 
        }
      }
    }
    catch(RuntimeException e) {
      // only if there is an error:
      e.printStackTrace();
    }
}

//This function is used to read the button state; it calls the Start and Stop functions whenever 
//The button state changes
public void controlEvent(ControlEvent theEvent) {
  println(theEvent.getController().getValue());
}

// function Start 
public void Start(int theValue) {
  println("a button event from Start: "+theValue);
  start = 1;
}

// function colorB will receive changes from 
// controller with name colorB
public void Stop(int theValue) {
  println("a button event from Stop: "+theValue);
  start = 0;
}