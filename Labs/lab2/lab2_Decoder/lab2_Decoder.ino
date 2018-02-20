//Initialize the pins
const int buttonPin = 2;
const int LEDPin = 12;
const int APin = 4;
const int BPin = 5;
const int CPin = 6;
const int DPin = 11;
const int EPin = 10;
const int FPin = 8;
const int GPin = 9;
const int DpPin = 7;
const int BuzzerPin = 13;

//lookup table for seven segment display digits
byte seven_seg_digits[11][8] ={
  {1,1,1,1,1,1,0,0},  //0
  {0,1,1,0,0,0,0,0},  //1
  {1,1,0,1,1,0,1,0},  //2
  {1,1,1,1,0,0,1,0},  //3
  {0,1,1,0,0,1,1,0},  //4
  {1,0,1,1,0,1,1,0},  //5
  {1,0,1,1,1,1,1,0},  //6
  {1,1,1,0,0,0,0,0},  //7
  {1,1,1,1,1,1,1,0},  //8
  {1,1,1,0,0,1,1,0},  //9
  {0,0,0,0,0,0,0,1},  //.
};

//lookup table for seven segment display letters
byte seven_seg_letters[26][8] = {
  {1,1,1,0,1,1,1,0},  //A
  {0,0,1,1,1,1,1,0},  //B
  {1,0,0,1,1,1,0,0},  //C
  {0,1,1,1,1,0,1,0},  //D
  {1,0,0,1,1,1,1,0},  //E
  {1,0,0,0,1,1,1,0},  //F
  {1,1,1,0,0,1,1,0},  //G same as 9
  {0,1,1,0,1,1,1,0},  //H
  {0,0,0,0,1,1,0,0},  //I
  {0,1,1,1,1,0,0,0},  //J
  {0,1,1,0,1,1,1,0},  //K same as H
  {0,0,0,1,1,1,0,0},  //L
  {1,0,1,0,1,0,0,0},  //M
  {0,0,1,0,1,0,1,0},  //N
  {1,1,1,1,1,1,0,0},  //O
  {1,1,0,0,1,1,1,0},  //P
  {1,1,1,0,0,1,1,0},  //Q
  {0,0,0,0,1,1,0,0},  //R
  {1,0,1,1,0,1,1,0},  //S
  {0,0,0,1,1,1,1,0},  //T
  {0,1,1,1,1,1,0,0},  //U
  {0,0,1,1,1,0,0,0},  //V
  {0,1,0,1,0,1,0,0},  //W
  {0,1,1,0,1,1,1,0},  //X same as H
  {0,1,1,1,0,1,1,0},  //Y
  {1,1,0,1,1,0,1,0}  //Z same as 2
};

//lookup table for Morse code letters
char *letters[] = {
  ".-",   //A
  "-...", //B
  "-.-.", //C
  "-..",  //D
  ".",    //E
  "..-.", //F
  "--.",  //G
  "....", //H
  "..",   //I
  ".---", //J
  "-.-",  //K
  ".-..", //L
  "--",   //M
  "-.",   //N
  "---",  //O
  ".--.", //P
  "--.-", //Q
  ".-.",  //R
  "...",  //S
  "-",    //T
  "..-",  //U
  "...-", //V
  ".--",  //W
  "-..-", //X
  "-.--", //Y
  "--.."  //Z
};

//lookup table for Morse code digits
char *numbers[] ={
  "-----",  //0
  ".----",  //1
  "..---",  //2
  "...--",  //3
  "....-",  //4
  ".....",  //5
  "-....",  //6
  "--...",  //7
  "---..",  //8
  "----."   //9
};

//references for Morse code other signals
char *fullStop = ".-.-.-";
char *comma = "--..--";
char *colon = "---...";
char *hyphenOrDash = "-....-";
char *error = "........";

int dotDelay = 200;

/**************************/

//Initialize variables
int lastButtonState;
int buttonState;
int ledState;
int lastPressTime;
int lastReleaseTime;
int i = 0; // i is for sequenceIndex
int refIndex; //refIndex is for looking up the letters/digits in the lookup tables
int match = 0;
int readingInput = 0;
int letterOrDigit = 0;
char letter;
char inputSequence[100] = {};
char tempSequence[100] = {};
const int lessThanDot = 100;
const int dotOrDashTime = 1000;
const int timeBetweenWords = 10000;
const int timeBetweenLetters = 3000;

void setup() {
  //Initialize the input and output pins
  pinMode(buttonPin, INPUT);

  pinMode(LEDPin,OUTPUT);
  pinMode(APin, OUTPUT);
  pinMode(BPin, OUTPUT);
  pinMode(CPin, OUTPUT);
  pinMode(DPin, OUTPUT);
  pinMode(EPin, OUTPUT);
  pinMode(FPin, OUTPUT);
  pinMode(GPin, OUTPUT);
  pinMode(DpPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  //read the signal from button pin in order to determine whether need to enter the readingInput mode or not
  int reading = digitalRead(buttonPin);

  if (reading == LOW) {  
    lastPressTime = millis();
  }

  while(digitalRead(buttonPin) == HIGH);

  //If the button has been pushed for longer than the threshhold time, then prepare to enter the readingInput mode
  //by setting readingInput to 1
  if(millis() - lastPressTime >= lessThanDot){
    readingInput = 1;
  }

  //Enter the reading Input mode
  while(readingInput == 1){
    int lastReleaseTime; 
    
    if (digitalRead(buttonPin) == LOW) {  
      lastPressTime = millis();
    }
    
    while(digitalRead(buttonPin) == HIGH);

    //Keep recording the input signal according to the interval for how long the button has been pushed
    //record the signals in a char array
    if ((millis() - lastPressTime) >  lessThanDot && (millis() - lastPressTime <= dotOrDashTime)) {
      inputSequence[i] = '.';
      i++;
      lastReleaseTime = millis();
    }else if((millis() - lastPressTime) > dotOrDashTime){
      inputSequence[i] = '-';
      i++;
      lastReleaseTime = millis();
    }else if(millis() - lastReleaseTime > timeBetweenLetters){
      inputSequence[i] = ' ';
      readingInput = 0;
    }else if(millis() - lastReleaseTime > timeBetweenWords){
      inputSequence[i] = '\0';
      readingInput = 0;
    }

    /*for(int index = 0; index < i; index++){
      Serial.print(inputSequence[index]);
    }*/
    Serial.print("This is inputSequence: ");
    Serial.println(inputSequence);

    
    for(int index = 0; index < i; index++){
      tempSequence[index] = inputSequence[index];  
      if(inputSequence[index] == ' '){
        i = index;
        break;
      }
    }
    Serial.print("This is tempSequence ");
    Serial.println(tempSequence);
    Serial.println("in the reading");
  }

        Serial.print("This is tempSequence: ");
        Serial.println(tempSequence);

  //If the readingInput is 0, and the tempSequence has reasonable content, enter the output mode
  while(readingInput == 0 && (tempSequence[0] == '.' || tempSequence[0] == '-')){
    char morseString[100];
    //Set the end index of tempSequence
    tempSequence[i+1] = '\0';
    //Convert the char array tempSequence to another char array morseString(which is like a String type)
    strcpy(morseString, tempSequence);

    //check if the content in tempSequence matches any of the Morse letter/digit encoding
    while(match == 0){
      for(int index = 0; index < 26; index++){
        Serial.print("This is morseString: ");
        Serial.println(morseString);
        Serial.print("This is letters[index]: ");
        Serial.println(letters[index]);

        //if there is a match, store the results, break the loop, and reset all relevant variables, 
        //in order to start another round of inputReading
        if(strcmp(morseString, letters[index])==0){
          Serial.println("Yay!!!!!!!!!!!!!!!!!!");
          match = 1;
          refIndex = index;
          letter = 'a'+refIndex;
          letterOrDigit = 0;
          i = 0;
          break;
        }
      }

      for(int index = 0; index < 10; index++){
        Serial.print("This is morseString: ");
        Serial.println(morseString);
        Serial.print("This is letters[index]: ");
        Serial.println(numbers[index]);

        //if there is a match, store the results, break the loop, and reset all relevant variables, 
        //in order to start another round of inputReading
        if(strcmp(morseString, numbers[index])==0){
          Serial.println("Yay!!!!!!!!!!!!!!!!!!");
          match = 1;
          refIndex = index;
          letter = '0'+refIndex;
          letterOrDigit = 1;
          i = 0;
          break;
        }
      }
        break;
      }

      //print the matched letter/digit
      if(match == 1){
        Serial.println(letter);
        if(letterOrDigit == 0){
          sevenSegDisplay(seven_seg_letters[letter-'a']);
        }else if(letterOrDigit == 1){
          sevenSegDisplay(seven_seg_digits[letter-'0']);
        }
        
        delay(3000);
        sevenSegOff();
        tempSequence[0] ='1'; 
        inputSequence[0] = '1';
        morseString[0] = "1";
      }

  //reset match and i
      match = 0;
      i = 0;
   }
}

//*****************************************

void sevenSegDisplay(char sequence[]){
  digitalWrite(APin, 1 - sequence[0]);
  digitalWrite(BPin, 1 - sequence[1]);
  digitalWrite(CPin, 1 - sequence[2]);
  digitalWrite(DPin, 1 - sequence[3]);
  digitalWrite(EPin, 1 - sequence[4]);
  digitalWrite(FPin, 1 - sequence[5]);
  digitalWrite(GPin, 1 - sequence[6]);
  digitalWrite(DpPin, 1 - sequence[7]);
}

void sevenSegOff(void){
  digitalWrite(APin, HIGH);
  digitalWrite(BPin, HIGH);
  digitalWrite(CPin, HIGH);
  digitalWrite(DPin, HIGH);
  digitalWrite(EPin, HIGH);
  digitalWrite(FPin, HIGH);
  digitalWrite(GPin, HIGH);
  digitalWrite(DpPin, HIGH);
}

void sevenSegOn(void){
  digitalWrite(APin, LOW);
  digitalWrite(BPin, LOW);
  digitalWrite(CPin, LOW);
  digitalWrite(DPin, LOW);
  digitalWrite(EPin, LOW);
  digitalWrite(FPin, LOW);
  digitalWrite(GPin, LOW);
  digitalWrite(DpPin, LOW);
}

