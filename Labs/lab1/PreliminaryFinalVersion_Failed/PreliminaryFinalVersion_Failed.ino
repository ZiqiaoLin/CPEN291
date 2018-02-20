const int redPin = 2;
const int greenPin = 4;
const int bluePin = 5;
const int newLEDPin = 11;
const int interruptPin = 3;

const int photoCellPin = A0;
const int buttonPin3 = 9; ///////
const int buttonPin4 = 10;///////

int CHANGEState = 0;
//Initialize a global variable in order to the timing of blinking intervals (1 sec in this case)
unsigned long blinkStartTime = 0;

//Initialize a gobal in order to track the timing of checking intervals (200 millisec in this case)
unsigned long switchCheckStartTime = 0;

//Initialize blinkRate and checkRate
unsigned long photoCheckStartTime = 0;

//Initialize other necessary variables variables
unsigned long interruptStartTime = 0;
unsigned long blinkRate = 1000;
unsigned long checkRate = 200;
//This timer is to keep track of the time/interval during the interrupt
int timer = 0;
int lightLevel = 0;

int currentState_3;
int currentState_4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Initialize the functionality of each digital pin
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(newLEDPin, OUTPUT);
}

//Initialize the functions for 4 different patterns
void Pattern_1(void);
void Pattern_2(void);
void Pattern_3(void);
void Pattern_4(void);
void goOut(void);

void loop() {

  //Call for different Patterns according to the state of the switch; This is like a nested Finite State Machine:
  //The main loop is a big State Machine, and the 4 patterns are
  int currentState_3 = digitalRead(buttonPin3);
  int currentState_4 = digitalRead(buttonPin4);

  Pattern_1(currentState_3, currentState_4);
}

void Pattern_1(int switchState_3, int switchState_4) {

  int currentState_3 = digitalRead(buttonPin3);
  int currentState_4 = digitalRead(buttonPin4);
  //The first while is used to loop through the whole pattern corresponding to the certain swithc state; there are 4 sub-whiles (for 4 different intervals) in this while.
  while(switchState_3 == currentState_3 && switchState_4 == currentState_4){
    
    //This while is for LED to stay the same color for one sec, meanwhile checking the state of the switch
    while(millis() - blinkStartTime <= blinkRate){
      //Use Serial Monitor to print values
      if(switchState_3 == HIGH && switchState_4 == LOW){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: ON OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }

        //Determine the states of the RGB LED
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == LOW && switchState_4 == HIGH){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF ON OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == HIGH && switchState_4 == HIGH){
        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: ON ON OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == LOW && switchState_4 == LOW){
        Serial.print("Switchs: OFF OFF   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }

        //check the switch every 200 milllisec, and decide whether to CHANGE to another pattern
        if(millis() - switchCheckStartTime >= checkRate){
          //Update the switchCheckStartTime for checking next time
         switchCheckStartTime = millis();
         //If the switch has been CHANGEd, update the checkStatTime and CHANGEState, and break the interval
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != LOW){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            //If the switch has been CHANGEd, then break the loop/interval;
            break;
         }
                  
         //Attach interrpt and check the photo cell every 200 ms
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        //Read the value of photoCellPin to determine the relative lightLevel
        lightLevel = analogRead(photoCellPin);

        //Turn off the LED if it is bright enough; otherwise adjust the LED lightness according to the ambient light level
        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
     }
    
    //Update the blinkStartTime for checking next time
    blinkStartTime = millis();
      if(CHANGEState == 1){
       //If the switch has been CHANGEd, then break the loop;
       //break the whole pattern   
      CHANGEState = 0; 
      break;
    }


/*****************Same logic as above for all other patterns (Patter_2 to Pattern_4)************************/

    while(millis() - blinkStartTime <= blinkRate){
      if(switchState_3 == HIGH && switchState_4 == LOW){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: ON ON OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }

        //Determine the states of the RGB LED
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == LOW && switchState_4 == HIGH){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF OFF ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,LOW);
      }else if(switchState_3 == HIGH && switchState_4 == HIGH){
        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: OFF ON ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,LOW);
      }else if(switchState_3 == LOW && switchState_4 == LOW){
        Serial.print("Switchs: OFF OFF   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != LOW){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
   if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }
    
     while(millis() - blinkStartTime <= blinkRate){
      while(millis() - blinkStartTime <= blinkRate){
      if(switchState_3 == HIGH && switchState_4 == LOW){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: ON ON ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }

        //Determine the states of the RGB LED
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,LOW);
      }else if(switchState_3 == LOW && switchState_4 == HIGH){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: ON OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == HIGH && switchState_4 == HIGH){
        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: ON OFF ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,LOW);
      }else if(switchState_3 == LOW && switchState_4 == LOW){
        Serial.print("Switchs: OFF OFF   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }
      
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != LOW){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

     while(millis() - blinkStartTime <= blinkRate){
       if(switchState_3 == HIGH && switchState_4 == LOW){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }

        //Determine the states of the RGB LED
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == LOW && switchState_4 == HIGH){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == HIGH && switchState_4 == HIGH){
        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }else if(switchState_3 == LOW && switchState_4 == LOW){
        Serial.print("Switchs: OFF OFF   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
      }
      
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != LOW){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

       //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
   if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }
  }
 }
}
}

/*void Pattern_2() {
  while(digitalRead(buttonPin3) == LOW && digitalRead(buttonPin4) == HIGH){
    while(millis() - blinkStartTime <= blinkRate){

        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF ON OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,HIGH);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != LOW || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

    while(millis() - blinkStartTime <= blinkRate){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: ON OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != LOW|| digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

     while(millis() - blinkStartTime <= blinkRate){
        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF OFF ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,LOW);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != LOW || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        timer = timer - 200;
        delay(200);
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

     while(millis() - blinkStartTime <= blinkRate){

        Serial.print("Switchs: OFF ON   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != LOW || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        timer = timer - 200;
        delay(200);
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }
  }
}

void Pattern_3() {
  while(digitalRead(buttonPin3) == HIGH && digitalRead(buttonPin4) == HIGH){
    while(millis() - blinkStartTime <= blinkRate){
        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: ON ON OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,HIGH);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

    while(millis() - blinkStartTime <= blinkRate){
        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: ON OFF ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,LOW);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,LOW);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

     while(millis() - blinkStartTime <= blinkRate){

        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: OFF ON ON   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,LOW);
        digitalWrite(bluePin,LOW);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
        
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }

     while(millis() - blinkStartTime <= blinkRate){

        Serial.print("Switchs: ON ON   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != HIGH || digitalRead(buttonPin4) != HIGH){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }

        //Attach interrpt and check the photo cell
        
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }
  }
}

void Pattern_4(){
   while(digitalRead(buttonPin3) == LOW && digitalRead(buttonPin4) == LOW){
    while(millis() - blinkStartTime <= blinkRate){
        Serial.print("Switchs: OFF OFF   ");
        Serial.print("LEDs: OFF OFF OFF   ");
        Serial.print("Photocell value: ");
        Serial.print(analogRead(A0)/1023*5*1000);
        Serial.print("   ");
        if(lightLevel >= 900){
          Serial.println("Light-Controlled LED: OFF");
        }else{
          Serial.println("Light-Controlled LED: ON");
        }
        
        digitalWrite(redPin,HIGH);
        digitalWrite(greenPin,HIGH);
        digitalWrite(bluePin,HIGH);
        if(millis() - switchCheckStartTime >= checkRate){
         switchCheckStartTime = millis();
          if(digitalRead(buttonPin3) != LOW || digitalRead(buttonPin4) != LOW){
            CHANGEState = 1;
            switchCheckStartTime = millis();
            break;
          }
        }
       //Attach interrpt and check the photo cell
       attachInterrupt(digitalPinToInterrupt(interruptPin), goOut, CHANGE);
       if(timer > 0){
        delay(200);
        timer = timer - 200;
        lightLevel = analogRead(photoCellPin);

        if(lightLevel > 900){
          digitalWrite(newLEDPin, LOW);
        }else{
          analogWrite(newLEDPin, (1023 - lightLevel)/4 );
        }
       }
    }

    blinkStartTime = millis();
    if(CHANGEState == 1){   
      CHANGEState = 0; 
      break;
    }
  }
}*/

void goOut(){
    timer = 15000;
}
