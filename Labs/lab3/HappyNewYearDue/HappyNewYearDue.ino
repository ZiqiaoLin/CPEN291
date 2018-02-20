/*
  LiquidCrystal Library - Blink

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and makes the
 cursor block blink.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
   * ends to +5V and ground
   * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalBlink

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, A0, A1, A2, A3);

byte newChar_1[8] = {
        B00100,
        B11111,
        B01010,
        B11111,
        B00100,
        B11111,
        B01110,
        B10101
};

byte newChar_2[8] = {
        B00000,
        B01111,
        B10000,
        B11111,
        B10010,
        B10010,
        B10010,
        B00010
};

byte newChar_3[8] = {
        B01000,
        B01111,
        B10010,
        B01111,
        B01010,
        B11111,
        B00010,
        B00010
};

byte newChar_4[8] = {
       B00010,
        B00010,
        B00011,
        B01111,
        B01011,
        B01010,
        B00010,
        B00010
};

byte newChar_5[8] = {
        B01000,
        B01000,
        B11110,
        B01010,
        B11111,
        B01100,
        B10010,
        B10010
};

byte newChar_6[8] = {
        B01111,
        B10100,
        B10100,
        B11111,
        B01110,
        B10101,
        B01100,
        B00100
};

byte newChar_7[8] = {
        B00100,
        B01110,
        B01110,
        B00100,
        B00100,
        B00100,
        B00000,
        B00100
};

byte newChar_8[8] = {
        B00001,
        B00010,
        B00100,
        B00011,
        B00010,
        B00111,
        B00000,
        B00111
};

byte newChar_9[8] = {
        B00100,
        B11101,
        B10110,
        B11111,
        B11110,
        B10101,
        B10100,
        B11111
};

byte newChar_10[8] = {
        B01000,
        B11110,
        B01000,
        B11111,
        B11110,
        B11111,
        B01010,
        B10001
};

byte newChar_11[8] = {
        B00010,
        B00111,
        B00010,
        B01111,
        B00010,
        B01011,
        B01110,
        B10001
};

byte newChar_12[8] = {
        B00000,
        B01110,
        B00100,
        B11110,
        B00100,
        B00100,
        B00100,
        B11111
};

byte newChar_13[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
};

byte newChar_14[8] = {
        B00100,
        B00100,
        B11111,
        B10101,
        B11111,
        B00100,
        B00100,
        B00100
};


void setup() {
    /*lcd.createChar(1, newChar_1);
    lcd.createChar(2, newChar_2);
    lcd.createChar(3, newChar_3);
    lcd.createChar(4, newChar_4);
    lcd.createChar(5, newChar_5);
    lcd.createChar(6, newChar_6);
    lcd.createChar(7, newChar_7);
    /*lcd.createChar(8, newChar_8);
    lcd.createChar(9, newChar_9);
    lcd.createChar(10, newChar_10);*/
    lcd.createChar(11, newChar_11);
    lcd.createChar(12, newChar_12);
    lcd.createChar(13, newChar_13);
    lcd.createChar(14, newChar_14);
    lcd.begin(16, 2);
    lcd.print("Happy New Year!!");
    
    lcd.setCursor(0, 4);

    lcd.write(byte(13));
    lcd.write(byte(13));
    lcd.write(byte(13));
    lcd.write(byte(13));
    lcd.write(byte(13));
    lcd.write(byte(13));
    lcd.write(byte(11));
    lcd.write(byte(12));
    lcd.print("Due");
    lcd.write(byte(14));
    lcd.print("....");
    
}

void loop() {
  // Turn off the blinking cursor:
  lcd.noBlink();
  delay(3000);
  // Turn on the blinking cursor:
  lcd.blink();
  delay(3000);
}


