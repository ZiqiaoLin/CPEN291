/*Name:     Yuhao Huang
  Date:     Feb 8th 2017
  Purpose:  Lab 5 Arduino part. This program controls the servo motor and ultrasonic range finder sensor. 
            The servo motor is set to the beginning point at the start of the loop, and then start to turn
            from 0 degree to 180 degree. As the servo motor turns, the sensor measures distance for every 
            degree. The degree and distance data is printed in the serial monitor. 
*/   

#include <Servo.h>

Servo myservo;                                            //Initialize a servo motor.

const int trigPin = 7;                                    //Define pin numbers of the ultrasonic sensor and the temperature sensor.       
const int echoPin = 6;
const int LM35Pin = A0;

long duration;                                            //sound wave travel time in microseconds.
float distance;                                           //distance from the ambient object to the sensor in centimeter.

float LM35temp;                                           //Calculated temperature read from the LM35 sensor in degree Celsius.
float LM35reading;                                        //Original data read from the temperature sensor.
 
float SpeedOfSound;                                       //Calculated speed of sound.
float time_cm;                                            //Calculated time needed for sound wave to travel 1 cm round trip in μs.

void setup() {
  pinMode(trigPin, OUTPUT);                               // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);                                // Sets the echoPin as an Input
  Serial.begin(9600);
  myservo.attach(10);                                     // Defines on which pin is the servo motor attached
}

void loop() {

  myservo.write(0);                                       // set the motor to the beginning point
                                          
  for(int i=0;i<=180;i++){                                // rotates the servo motor from 0 to 180 degrees
  myservo.write(i);
  delay(10);
  distance = calculateDistance();                         // Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree

  Serial.print("Degree:");                                // Print which degree the servo motor is currently at.
  Serial.print(i);
  Serial.print("______");
  
  if (distance >= 200 || distance <= 2){
       Serial.print("Distance = ");                       // Print distance data at this degree.
       Serial.println("Out of range ／ no object");    
     }                                                    // If the distance calculated is beyond the best performance range, it prints special message.
  else {
       Serial.print("Distance = ");
       Serial.print(distance);
       Serial.println(" cm");
     }            
  }
                                          
  for(int i=180;i>0;i--){                                 // Repeats the previous lines from 180 back to 0 degree.
  myservo.write(i);
  delay(10);
  distance = calculateDistance();
  
  Serial.print("Degree:");
  Serial.print(i);
  Serial.print("______");
  
  if (distance >= 200 || distance <= 2){
       Serial.print("Distance = ");
       Serial.println("Out of range / no object");
     }
  else {
       Serial.print("Distance = ");
       Serial.print(distance);
       Serial.println(" cm");
     }
  }
}

/*
 * *This function calculates the distance from an object in front of the sensor to the sensor itself. 
 */
float calculateDistance(){ 

  read_LM35_Temp();                                        // calls to function to read from the temperature sensor.alculate the speed of sound from ambient temperature
  SpeedOfSound = 331.5 + (0.6 * LM35temp);                 // calculate the speed of sound from ambient temperature
  time_cm = 20000 / SpeedOfSound;                          // calculate the time it takes for sound wave to travel (round trip) per cm in μs
  
  digitalWrite(trigPin, LOW);                              // clear the trig pin to low.
  delayMicroseconds(20);
  
                                              
  digitalWrite(trigPin, HIGH);                             // trigger the sensor. 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);                       // Reads the echoPin, returns the sound wave travel time in μs
  distance= duration / time_cm;                            // calculate the distance.
  return distance;
}


/*
 * This function reads from the temperature sensor and calculate the ambient temperature. 
 * Retrieved from lad 4 code.
 */
void  read_LM35_Temp(){    
  LM35reading = analogRead(LM35Pin);
  LM35temp = LM35reading /9.31;
  }
