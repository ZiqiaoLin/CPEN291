
const int D4 = A0;
const int D5 = A1;
const int D6 = A2;
const int D7 = A3;
const int RS = 13;
const int E  = 12;


void LCD_Write(byte cmd,int cmddta){
  digitalWrite(RS,cmddta); //Change RS for command/data
  digitalWrite(D4,(cmd & 0b00010000)>>4);
  digitalWrite(D5,(cmd & 0b00100000)>>5);
  digitalWrite(D6,(cmd & 0b01000000)>>6);
  digitalWrite(D7,(cmd & 0b10000000)>>7);
  delay(1);
  digitalWrite(E,HIGH);
  delay(1);
  digitalWrite(E,LOW);
  delay(0.5);
  digitalWrite(D4,(cmd & 0b00000001)>>0);
  digitalWrite(D5,(cmd & 0b00000010)>>1);
  digitalWrite(D6,(cmd & 0b00000100)>>2);
  digitalWrite(D7,(cmd & 0b00001000)>>3);
  delay(1);
  digitalWrite(E,HIGH);
  delay(1);
  digitalWrite(E,LOW);
  delay(1);
}


void LCD_Char(char character){
  LCD_Write(character,1);
}

void LCD_String(String str){
 char *cstr = new char[str.length()];
 strcpy(cstr, str.c_str());
 while(*cstr) LCD_Char (*cstr++);
}

void LCD_Int(unsigned long n){
  char c[10];
  sprintf(c, "%d", n);
  for (int i=0;c[i] > 0 ;i++) LCD_Char(c[i]);
}



void LCD_Clear(){
  LCD_Write(0x01,0);
}

void LCD_Init(){
  LCD_Write(0x33,0);
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

void LCD_SetCursor(int col, int row){
   if (col == 0)LCD_Write((row | 0x80),0);
   if (col == 1)LCD_Write((row | 0x80)+ 0x40,0);
   if (col == 2)LCD_Write((row | 0x80)+ 0x10,0);
   if (col == 3)LCD_Write((row | 0x80)+ 0x50,0);
   delay(1);

}



void setup() {
  Serial.begin(9600);
  pinMode(RS, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  delay(50);
  LCD_Init();
}

void loop() {
  LCD_SetCursor(1, 0);
  LCD_Int(3478);
  //LCD_String("Hello World!");
  /*LCD_SetCursor(1,1);
  LCD_Int(3478);
  LCD_String("m!");*/
 }
