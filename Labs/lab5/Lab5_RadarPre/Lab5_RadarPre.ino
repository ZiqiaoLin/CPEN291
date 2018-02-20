/*Name:     Yuhao Huang
  Date:     Feb 8th 2017
  Purpose:  Lab 5 Arduino part. This program controls the servo motor and ultrasonic range finder sensor. 
            The servo motor is set to the beginning point at the start of the loop, and then start to turn
            from 0 degree to 180 degree. As the servo motor turns, the sensor measures distance for every 
            degree. The degree and distance data is printed in the serial monitor. 
*/   

#include <Servo.h>. 
// Defines Tirg and Echo pins of the Ultrasonic Sensor
const int trigPin = 7; //Define pin numbers of the ultrasonic sensor and the temperature sensor.       
const int echoPin = 6;
const int LM35Pin = A0;

// Variables for the duration and the distance
long duration; //sound wave travel time in microseconds.
float distance;
float LM35temp; //Calculated temperature read from the LM35 sensor in degree Celsius.
float LM35reading; //Original data read from the temperature sensor.
 
float SpeedOfSound; //Calculated speed of sound.
float time_cm; //Calculated time needed for sound wave to travel 1 cm round trip in μs.

int input = 0;
int stateFlag = 1;

int servoDelayTime = 30;

Servo myServo; //Initialize a servo motor.

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(10); // Defines on which pin is the servo motor attached
}

void loop() {
  //Check the input from Processing via the Serial port
  if(Serial.available() > 0){
     input = Serial.read();
  } 

  //Excecute different commands according to the values 
  //If input us between 5 and 45, adjust the delay time of the servo rotation
  if(input >= 5 && input <= 45){
    servoDelayTime = 60 - input;
  }else if(input > 0){
    //If the input is 1, change stateFlag to 1
     stateFlag = 1;
  }else if(input == 0){
    //If the input is 1, change stateFlag to 1
     stateFlag = 0;
  }

  //If the stateFlag is 1, start the program
    if(stateFlag == 1){
      // rotates the servo motor from 15 to 165 degrees
      for(int i=10;i<=170;i++){  
        myServo.write(i);
        delay(servoDelayTime);
        distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
        
        Serial.print(i); // Sends the current degree into the Serial Port
        Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
        Serial.print(distance); // Sends the distance value into the Serial Port
        Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
        Serial.print(LM35temp);
        Serial.println();

      //Check if there are new incoming bytes; if there is a 0 is received, pause the program
        if(Serial.available() > 0){
          input = Serial.read();
          if(input == 0){
            stateFlag = 0;
            //keep checking if there are new incoming bytes; if there is a 1 is received, resume the program
             while(1){
              if(Serial.available() > 0){
                input = Serial.read();
              }
    
              if(input == 1){
                stateFlag = 1;
                break;
              }
            }
          }
        }
      }

      //If input us between 5 and 45, adjust the delay time of the servo rotation
      if(input >= 5 && input <= 45){
        servoDelayTime = 60 - input;
      }
      // Repeats the previous lines from 165 to 15 degrees
      for(int i=170;i>10;i--){  
        myServo.write(i);
        delay(servoDelayTime);
        distance = calculateDistance();
        
        Serial.print(i); // Sends the current degree into the Serial Port
        Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
        Serial.print(distance); // Sends the distance value into the Serial Port
        Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
        Serial.print(LM35temp);
        Serial.println();

        if(Serial.available() > 0){

         //Check if there are new incoming bytes; if there is a 0 is received, pause the program
         input = Serial.read();
          if(input == 0){
            stateFlag = 0;
            //keep checking if there are new incoming bytes; if there is a 1 is received, resume the program
             while(1){
              if(Serial.available() > 0){
                input = Serial.read();
              }
    
              if(input == 1){
                stateFlag = 1;
                break;
              }
            }
          }
        }
      }
    }
    //If the stateFlag is 0, pause the program
    else if(stateFlag == 0){
       //keep checking if there are new incoming bytes; if there is a 1 is received, resume the program
        while(1){
          if(Serial.available() > 0){
            input = Serial.read();
          }

          if(input == 1){
            break;
          }
        }
    }
}

/*
 * *This function calculates the distance from an object in front of the sensor to the sensor itself. 
 */
float calculateDistance(){ 
  float distance_pre = distance;
  
  read_LM35_Temp();                                        // calls to function to read from the temperature sensor.alculate the speed of sound from ambient temperature
  SpeedOfSound = 331.5 + (0.6 * LM35temp);                 // calculate the speed of sound from ambient temperature
  time_cm = 20000 / SpeedOfSound;                          // calculate the time it takes for sound wave to travel (round trip) per cm in μs
  
  digitalWrite(trigPin, LOW);                              // clear the trig pin to low.
  //delayMicroseconds(30);
  delay(1);
                                              
  digitalWrite(trigPin, HIGH);                             // trigger the sensor. 
  //delayMicroseconds(60);
  delay(1);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);                       // Reads the echoPin, returns the sound wave travel time in μs
  distance= duration / time_cm;                            // calculate the distance.
  
  if(distance > 1 && distance < 200){
    return distance;
  }else{
    return distance_pre;
  }
  
}

/*
 * This function reads from the temperature sensor and calculate the ambient temperature. 
 * Retrieved from lad 4 code.
 */
void  read_LM35_Temp(){
  float temp_pre = LM35temp;
  
  LM35reading = analogRead(LM35Pin);
  LM35temp = LM35reading /9.31;

  //This functions as a filter; The LM35 temperature sensor sometimes reads a wrong value
  //If this happens, this filter should filter out that value and use the previous value
  //instead
  if(millis() > 500 && abs(temp_pre - LM35temp) >= 0.5){
    LM35temp = temp_pre;
  }
}
