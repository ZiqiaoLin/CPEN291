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

char *fullStop = ".-.-.-";
char *comma = "--..--";
char *colon = "---...";
char *hyphenOrDash = "-....-";
char *error = "........";

int dotDelay = 200;
void setup() {
  // put your setup code here, to run once:
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
  
  sevenSegOff();
  
  char ch;
  if(Serial.available()){
    sevenSegOff();
    ch = Serial.read();
    
    Serial.print(ch);
    
    if(ch>='a' && ch<='z'){
       sevenSegDisplay(seven_seg_letters[ch-'a']);
       morseCodeSequence(letters[ch-'a']);
     }else if(ch >= 'A' && ch <='Z'){
        sevenSegDisplay(seven_seg_letters[ch-'A']);
        morseCodeSequence(letters[ch-'A']);
     }else if(ch >='0' && ch <= '9'){
        sevenSegDisplay(seven_seg_letters[ch-'0']);
        morseCodeSequence(numbers[ch-'0']);
     }else if (ch == ' '){
        sevenSegOff();
        delay(7 * dotDelay );  // space between words
     }else if ( ch == '.' ){
        sevenSegDisplay(seven_seg_digits[10]);
        morseCodeSequence(fullStop);
     }else if (ch == ','){
        sevenSegOff();
        morseCodeSequence(comma);
     }else if (ch == ':'){
        sevenSegOff();
        morseCodeSequence(colon);
     }else if( ch == '-'){
        sevenSegOff();
        morseCodeSequence(hyphenOrDash);
     }else{
        sevenSegOff();
        morseCodeSequence(error);
     }
  }else{
    sevenSegOff();
  }

  Serial.println();

      
  // put your main code here, to run repeatedly:
  /*
  morseCodeSequence("...");  //s
  morseCodeSequence("---");  //o
  morseCodeSequence("...");  //s
  */
  
}

//*****************************************

void morseCodeSequence(char *sequence)
{
  int i = 0;
  while(sequence[i] != NULL){
    printDotOrDash(sequence[i]);
    i++;
    }
    delay(3*dotDelay);  // space between letters in a word
}

//*****************************************

void printDotOrDash(char dotOrDash){
    digitalWrite(LEDPin,HIGH);
    if(dotOrDash == '.'){
      //Enable the buzzer
        tone(BuzzerPin, 500);
        delay(dotDelay);
    }else{
      //Enable the buzzer
        tone(BuzzerPin, 200);
        delay(3*dotDelay);   // dashSpace
    }
    digitalWrite(LEDPin,LOW);
    noTone(BuzzerPin);
    delay(dotDelay);
}

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

/*void sevenSegOn(void){
  digitalWrite(APin, LOW);
  digitalWrite(BPin, LOW);
  digitalWrite(CPin, LOW);
  digitalWrite(DPin, LOW);
  digitalWrite(EPin, LOW);
  digitalWrite(FPin, LOW);
  digitalWrite(GPin, LOW);
  digitalWrite(DpPin, LOW);
}*/

/*void buzzer(char sequence[]){
  int i = 0;
  while(sequence[i] != NULL){
    if(sequence[i] == '.'){
      tone(buzzerPin, 600);
    }else if(sequence[i] == '-'){
      tone(buzzerpin, 200);
    }
    i++;
  }
}*/

