const int rows = 4; //number of rows of keypad
const int columns = 3; //number of columnss of keypad 
const int Output[rows] = {8,7,6,5}; //array of pins used as output for rows of keypad
const int Input[columns] = {4,3,2}; //array of pins used as input for columnss of keypad
int colNum = 0, rowNum = 0; // two int type to determine the specified column number and row number 
boolean replay = true;
boolean difficultiesFlag = 0;



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
}

void loop()

{
  if(replay){
          Serial.println("enjoy the game !");
          guessTheNum();
            }           
}
//*******************************************************


/*
 * This method is same function as keypad library, which only will determine the number 
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
//*************************************************************
   /*
    * This method is the guess the number game, and the game will ter
    * 
    */
void guessTheNum(){
  int range = difficulties();   // get difficulties level
  do{
    Serial.print("checking");
    }while(!difficultiesFlag);
  int attemptNum = 5;           // attempt number
  int counter = 0;              // set counter from 0
  Serial.print("take a guess\n");
  int guessNum = 0;             // define a guess number
  int ranNum = rand()%range;    // get ranNum in the range from difficulties level
  bool judge = 0;               // set a flag which will determine when will jump off the loop

  do{                           // the loop to get the result from game
    if( counter <= attemptNum){     // if  counter <= attemptNum, go inside the condition

       guessNum = getNum();
      if(guessNum == ranNum){      //  guess number == random number, the game over, user won
        Serial.println("You won!");
        judge = 1;
        difficultiesFlag = 0;
        }
      if(guessNum > ranNum){      //  guess number > random number, notice user to guess lower
        Serial.println(guessNum);
        Serial.println("Guess Lower");
        counter++;                // counter + 1
        }
      if(guessNum < ranNum){      //  guess number < random number, notice user to guess higher
        Serial.println("Guess Higher");
        counter++;                // counter + 1
        }
    }else{                        // if counter > attempt number, game over, and user lose
      Serial.println("You lost!");
      judge = 1;
      difficultiesFlag = 0;
      }  
    }while(!judge);

    return 0;
  }
//*****************************************************************
/*
 * getNum() method will return a number which user press on keypad
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
                    Serial.print("the total number is  ");
                    Serial.println(num);
                  break;
                  
                  case 11:                       // case 11 represent "0" in keypad
                    num = num * 10 + 0;          // num + 0 in total number
                    Serial.print("the total number is  ");
                    Serial.println(num);
                  break;
                  
                  case 10:                       // case 10 represent "*" in keypad which will make one backpace
                    num = num/10;                // use total number / 10 will eliminate the last digit in num
                    Serial.print("the total number is  ");
                    Serial.println(num);
                    break;
                }   
                commandKey = keypad();           // receive next input from keypad
             }
             Serial.print("the number you guess is  ");
             Serial.println(num);
             return num;

}
//*****************************************************

/*
 * this method is for user to choose different difficuties for the guess number game
 * return int option, which represent the number that will be rand() rest.
 */
int difficulties(){
  Serial.print("difficulties choose:\n");  // print difficulties for user
  Serial.print("1. range 0 - 20\n");   // difficulty for 0 - 20
  Serial.print("2. range 0 - 50\n");   // difficulty for 0 - 50
  Serial.print("3. range 0 - 99\n");   // difficulty for 0 -99
  int option = 0;   // define a number as int
  int command = 0;
  do{
    command = keypad();
    }while(command<1 || command > 3);
  switch(command){     
    case 1: option = 21; break;    //which can create randum number 0 - 20
    case 2: option = 51; break;  //which can create randum number 0 - 50
    case 3: option = 100;break;    //which can create randum number 0 - 99
    }
    difficultiesFlag = 1;
    return option;  // return number
  }
