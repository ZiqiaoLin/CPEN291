/*
 * Author: Yuxiang Huang, Ziqiao Lin, Yuhao Huang 
 * Date: January 26, 2017
 * Purpose: Implement the "Guess the Number" Game; learn to 
 * use keypad and LCD without library; Finish the lab 3 of
 * CPEN 291
 */

//Pins for keyboard
const int rows = 4; //number of rows of keypad
const int columns = 3; //number of columnss of keypad 
const int Output[rows] = {8,7,6,5}; //array of pins used as output for rows of keypad
const int Input[columns] = {4,3,2}; //array of pins used as input for columnss of keypad

//Pins for LCD
const int D4 = A0;
const int D5 = A1;
const int D6 = A2;
const int D7 = A3;
const int RS = 13;
const int EN  = 12;

//Other related parameters
int colNum = 0, rowNum = 0; // two int type to determine the specified column number and row number 
boolean replay = true;
boolean difficultyFlag = 0;

void setup()

{
  for (byte i = 0; i < rows; i++) //for loop used to make pin mode of outputs as output
  {
    pinMode(Output[i], OUTPUT);
  }
  for (byte s = 0; s < columns; s++) //for loop used to makk pin mode of inputs as inputpullup
  {
    pinMode(Input[s], INPUT_PULLUP);
  }
  Serial.begin(9600); //to use serial monitor we set the buad rate

  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  delay(50);
  LCD_Initial();
}

void loop()

{
  //Initialize the cursor position at first
  LCD_SetCursor(0,0);

  //refresh the screen and start/restart a new round of game
  if(replay){
    LCD_Clear();
    LCD_String("enjoy the game !");
    delay(2000);
    guessTheNum();
  }           
}

/*****************************************************************************/

/*
 * This function is same function as keypad library, which only will determine the number 
 * return number = row * 3 + column + 1 
 */
int keypad() 

{

  bool no_press_flag = 0;                             // flag used to ensure no button is pressed
  for (byte x = 0; x < columns; x++)                  // for loop used to read all inputs of keypad to ensure no button is pressed
  {
    if (digitalRead(Input[x]) == HIGH);               //read evry input if high continue else break;
    else
      break;
    if (x == (columns - 1))                           //determine x value to make sure if no button is pressed
    {
      no_press_flag = 1;                              // set flag as 1
      colNum = 0;
      rowNum = 0;
    }
  }
  if (no_press_flag == 1)                             //if no button is pressed
  {
    for (byte r = 0; r < rows; r++)                   //for loop used to make all output as low
      digitalWrite(Output[r], LOW);
    for (colNum = 0; colNum < columns; colNum++)      // for loop to check if one of inputs is low
    {
      if (digitalRead(Input[colNum]) == HIGH)         //if specific input is remain high (no press on it) continue
        continue;
      else //if one of inputs is low
      {
        for (rowNum = 0; rowNum < rows; rowNum++)     //for loop used to determin the row number which make switch on
        {
          digitalWrite(Output[rowNum], HIGH);         //make specified output as HIGH then determine which row number
          if (digitalRead(Input[colNum]) == HIGH)     //makes the column number HIGH
          {
            no_press_flag = 0;                        //reset the flag to low;
            for (byte w = 0; w < rows; w++)           // make all outputs as low
              digitalWrite(Output[w], LOW);
            return rowNum * 3 + colNum + 1;           //return number of button
          }
        }
      }
    }
  }
  return 50;
}


/*********************************************************************/
//The following functions are for the keypad
   /*
    * This function is the guess the number game; the game will termintate if the user wins 
    * or loses, and start a new round afterwards
    */
void guessTheNum(){
  int range = difficulty();   // get difficulty level
  do{
    LCD_Clear();
    delay(200);
    LCD_SetCursor(0, 0);
    LCD_String("checking");
    delay(800);
    }while(!difficultyFlag);
  int attemptNum = 5;           // attempt number
  int counter = 0;              // set counter from 0
  LCD_SetCursor(1, 0);
  LCD_String("take a guess");
  LCD_Clear();
  LCD_SetCursor(0,0);
  LCD_String("take a guess");
  int guessNum = 0;             // define a guess number
  int ranNum = rand()%range;    // get ranNum in the range from difficulty level
  bool judge = 0;               // set a flag which will determine when will jump off the loop

  do{                           // the loop to get the result from game
    if( counter <= attemptNum){     // if  counter <= attemptNum, go inside the condition

       guessNum = getNum();
      if(guessNum == ranNum){      //  guess number == random number, the game over, user won
        LCD_Clear();
        delay(200);
        LCD_SetCursor(0, 0);
        LCD_String("You won!");
        delay(2000);
        judge = 1;
        difficultyFlag = 0;
        }
      if(guessNum > ranNum){      //  guess number > random number, notice user to guess lower
        LCD_String(" Guess Lower");
        counter++;                // counter + 1
        }
      if(guessNum < ranNum){      //  guess number < random number, notice user to guess higher
        LCD_String(" Guess Higher");
        counter++;                // counter + 1
        }
    }else{                        // if counter > attempt number, game over, and user lose
      LCD_Clear();
      delay(200);
      LCD_SetCursor(0, 0);
      LCD_String("You lost!");
      delay(2000);

      LCD_Clear();
      delay(200);
      LCD_SetCursor(0, 0);
      LCD_String("The number is: ");
      LCD_SetCursor(1, 0);
      LCD_Int(ranNum);
      delay(2000);
      
      judge = 1;
      difficultyFlag = 0;
      }  
    }while(!judge);

    return 0;
  }
//*****************************************************************//

/*
 * getNum() function will return a number which user press on keypad
 */
int getNum(){
  int num = 0;  // define a number as num which will be returned to game
  int commandKey = 0;   // this number will receive command from keypad
          commandKey = keypad();

          /*
           * this loop will continue to receive input until user press "#" 
           */
          while(commandKey !=12){                // case 12 means "#" in keypad, which represent "enter" case
                switch (commandKey)              //switch used to specify which button
                {
                  case 1:                        // case 1 to case9 represent 1 - 9 in keypad
                  case 2:
                  case 3:
                  case 4:
                  case 5:
                  case 6:
                  case 7:
                  case 8:
                  case 9:
                    num = num*10 + commandKey;  // when we receive 2 or more number, we need to represent as format xxxxx
                    LCD_Clear();
                    LCD_SetCursor(0, 0);
                    LCD_String("your guess:  ");
                    LCD_SetCursor(1, 0);
                    LCD_Int(num);
                  break;
                  
                  case 11:                       // case 11 represent "0" in keypad
                    num = num * 10 + 0;          // num + 0 in total number
                    LCD_Clear();
                    LCD_SetCursor(0, 0);
                    LCD_String("your guess:  ");
                    LCD_SetCursor(1, 0);
                    LCD_Int(num);
                  break;
                  
                  case 10:                       // case 10 represent "*" in keypad which will make one backpace
                    num = num/10;                // use total number / 10 will eliminate the last digit in num
                    LCD_Clear();
                    LCD_SetCursor(0, 0);
                    LCD_String("your guess:  ");
                    LCD_SetCursor(1, 0);
                    LCD_Int(num);
                    break;
                }   
                commandKey = keypad();           // receive next input from keypad
             }
              LCD_Clear();
              delay(500);
              LCD_SetCursor(0, 0);
              LCD_String("your guess:  ");
              LCD_SetCursor(1, 0);
              LCD_Int(num);
              return num;

}

/*
 * this function is for user to choose different difficuties for the guess number game
 * return int option, which represent the number that will be rand() rest.
 */
int difficulty(){
  
  //Clear the LCD for a new display
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_String("difficulty Lv: ");  // print difficulty for user
  delay(2000);
  
  LCD_Clear();
  delay(200);
  LCD_SetCursor(0, 0);
  LCD_String("1: range 0 - 20");   // difficulty level 1 for 0 - 20
  delay(2000);
  
  LCD_Clear();
  delay(200);
  LCD_SetCursor(0, 0);
  LCD_String("2: range 0 - 50");   // difficulty level 2 for 0 - 50
  delay(2000);
  
  LCD_Clear();
  delay(200);
  LCD_SetCursor(0, 0);
  LCD_String("3: range 0 - 99");   // difficulty level 3 for 0 -99
  delay(2000);

  //Clear the LCD screen for the display later
  LCD_Clear();
  int option = 0;   // define a number as int
  int command = 0;

  //Get the input from the keypad, and show the difficulty level on the LCD screen
  do{
    command = keypad();
    }while(command<1 || command > 3);
  switch(command){     
    case 1: option = 21; 
            LCD_String("Lv 1"); 
            delay(1000);
            break;    //which can create randum number 0 - 20
    case 2: option = 51;
            LCD_String("Lv 2");
            delay(1000);
            break;     //which can create randum number 0 - 50
    case 3: option = 100;
            LCD_String("Lv 3"); 
            delay(1000);
            break;     //which can create randum number 0 - 99
    }
    difficultyFlag = 1;
    return option;  // return number
}

/*********************************************************************/
/**********Funttions For LCD******************************************/
/*********************************************************************/

/*This functino is used to write commands/data to the LCD*/
void LCD_Write(byte cmd,int data_cmd){
  
  //At first, set the RS pin to corresponding value in order to receive instructions or data from the Arduino
  //Set RS pin to 1 to receive data, or 0 to receive instructions
  digitalWrite(RS,data_cmd); 
  
  //Since we are using 4-bit mode, we are using buses 4 to 7, and 
  //we need to transmit the 8-bit command/data in two steps
  //First, we send the 4 most significant bits  to bus 4 to 7 as following*/
  digitalWrite(D4,(cmd & 0b00010000)>>4);
  digitalWrite(D5,(cmd & 0b00100000)>>5);
  digitalWrite(D6,(cmd & 0b01000000)>>6);
  digitalWrite(D7,(cmd & 0b10000000)>>7);
  
  //delay 1 ms to wait for the completion of data/instruction tranmission
  delay(1);
  
  //Set the Enable pin to HIGH, and then LOW, to indicate the start of a new transmission
  digitalWrite(EN,HIGH);
  delay(1);
  digitalWrite(EN,LOW);
  delay(1);

  //Start a new set of transmission just as that above
  digitalWrite(D4,(cmd & 0b00000001)>>0);
  digitalWrite(D5,(cmd & 0b00000010)>>1);
  digitalWrite(D6,(cmd & 0b00000100)>>2);
  digitalWrite(D7,(cmd & 0b00001000)>>3);
  delay(1);
  digitalWrite(EN,HIGH);
  delay(1);
  digitalWrite(EN,LOW);
  delay(1);
}

/*This function is used to print a char on the LCD screen;
  It is mainly used in other functions*/
void LCD_Char(char character){
  LCD_Write(character,1);
}

/*This function is used to print a string on the LCD screen*/
void LCD_String(String str){
 char *cstr = new char[str.length()];
 strcpy(cstr, str.c_str());
 while(*cstr) LCD_Char (*cstr++);
}

/*This functions is used to print a number on the LCD scree*/
void LCD_Int(unsigned long n){
  char c[10];
  sprintf(c, "%d", n);
  for (int i=0;c[i] > 0 ;i++) LCD_Char(c[i]);
}

/*This function is used to clear the LCD screen*/
void LCD_Clear(){
  LCD_Write(0x01,0);
}

/*This function is used to send initialization commands to the LCD*/
void LCD_Initial(){
  LCD_Write(0x33,0);
  //Delay 16 ms after sending every instruction to ensure all the instructions 
  //have been executed properly
  delay(16);
  LCD_Write(0x32,0);
  delay(16);
  LCD_Write(0x28,0);
  delay(16);
  LCD_Write(0x0C,0);
  delay(16);
  LCD_Write(0x06,0);
  delay(16);
  LCD_Write(0x01,0);
  delay(16);
}

/*This function is used to set the position of the cursor on the LCD*/
void LCD_SetCursor(int col, int row){
   if (col == 0)LCD_Write((row | 0x80),0);
   if (col == 1)LCD_Write((row | 0x80)+ 0x40,0);
   if (col == 2)LCD_Write((row | 0x80)+ 0x10,0);
   if (col == 3)LCD_Write((row | 0x80)+ 0x50,0);
   delay(1);
}

