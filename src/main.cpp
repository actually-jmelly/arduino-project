#include <Arduino.h>

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int digitPins[4] = {2, 3, 4, 5};

int numbers[10] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 144}; // This one is different from my previous numbers m-arrays. It's not in bits but in a 0-255 code.
int numbersDecimal[10] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16};
int numToDisplay = 1234;

void setup() {
  // Initialize our pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for(int i = 2; i < 6; i++){
    pinMode(i, OUTPUT);
  }
}

void flashDigit(int digitToFlash){
  // int digitToFlash is not the actual pin number, but the named number of the digit from 1 to 4.

  // First, clear the frame (I often call frames digits) that we want to flash.
  digitalWrite(digitPins[digitToFlash - 1], LOW);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);

  // Set the frame we want to flash to HIGH, and the other frames to LOW
  for(int i = 0; i < 4; i++){
    if(i == digitToFlash - 1){
      digitalWrite(digitPins[i], HIGH);
    } else {
      digitalWrite(digitPins[i], LOW);
    }
  }
}

void sendRenderToShiftRegister(int numData){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 255);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numbersDecimal[numData]);
    
    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, MSBFIRST, 255);
    digitalWrite(latchPin, LOW);
}

void displayInt(int integer){
  if (integer > 9999 or integer < 0){
    // Error code
  } else {
    int thousands = integer / 1000;
    int thousandsRemain = integer % 1000;
    int hundreds = thousandsRemain / 100;
    int hundredsRemain = thousandsRemain % 100;
    int tens = hundredsRemain / 10;
    int ones = hundredsRemain % 10;
    flashDigit(1);
    sendRenderToShiftRegister(thousands);
    flashDigit(2);
    sendRenderToShiftRegister(hundreds);
    flashDigit(3);
    sendRenderToShiftRegister(tens);
    flashDigit(4);
    sendRenderToShiftRegister(ones);
  }
}

void loop() {
    displayInt(numToDisplay);
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

  An interesting note: any digit or figure which is entirely lit up except for one segment will fall on a... What do you call it... A primary bit number? Something like 1, 2, 4, 8, 16 etc
  I now think this may be incorrect because reasons
*/