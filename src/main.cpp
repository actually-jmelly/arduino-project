#include <Arduino.h>

//**************************************************************//
//  Name    : shiftOutCode, Hello World
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis
//  Date    : 25 Oct, 2006
//  Modified: 23 Mar 2010
//  Version : 2.0
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255
//****************************************************************
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for(int i = 2; i < 6; i++){
    pinMode(i, OUTPUT);
  }
}
void loop() {
    // take the latchPin low so
    // the LEDs don't change while you're sending in bits:
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, 249);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
}

/*

  COMBINATIONS NEEDED TO ACHEIVE NUMBERS

  First column: Resulting number or graphic
  Second column: Bit value

  ~ With decimal points ~
  0. 64
  1. 121
  2. 36
  3. 48
  4. 25
  5. 18
  6. 2
  7. 88 or 120
  8. 0
  9. 16 or 24

  C. 70
  c. 39
  d. 33
  F. 14
  H. 9
  h. 11
  J. 97
  L. 71
  P. 12
  y. 17
  U. 65
  u. 99

  -. 63
  _. 119

  ~ Without decimal points ~
  0 192
  1 249
  2 164
  3 176
  4 153
  5 146
  6 130
  7 248
  8 128
  9 144

  An interesting note: any digit or figure which is entirely lit up except for one segment will fall on a... What the heck do you call it... A primary bit number? Something like 1, 2, 4, 8, 16 etc
*/