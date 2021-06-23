#include <Arduino.h>

// Pin values
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int digitPins[4] = {2, 3, 4, 5};
int ldrPin = A0;

// Make a multiplexed array that holds all our figure sets
// That's a confusing sentence, right? I'll need to fix that
int figures[3][10] = {
  {192, 249, 164, 176, 153, 146, 130, 248, 128, 144}, // Numbers
  {64, 121, 36, 48, 25, 18, 2, 120, 0, 16}, // Numbers with a decimal point
  {137, 193, 137, 193, 0, 0, 0, 0, 0, 0} // An error code: 'HUHU'
};
// If I posted this to GitHub
// And you know where 'HUHU' comes from
// Please, please tell me
// It was in diagrams for 4 digit 7 segment displays
// And I'd love to know what it means

// Maybe just rename these to xIndex
int numsInFiguresIndex = 0;
int decimalNumsInFiguresIndex = 1;
int errorInFiguresIndex = 2;

// This int treats its last digit as a decimal value.
// For example, 3564 = 356.4

// Do NOT set this int too high, even though there is an error code for it.
// BEWARE BUFFER OVERFLOW
int numToDisplay = 1234;
// Hold the value of the light sensor
int sensorValue = 0;

void setup() {
  // Initialize our pins
  // ldr seems like it doesn't need to be initialized.
  // Probably because it's an analog pin.
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

void sendRenderToShiftRegister(int arrayToDisplay, int numData){
    // I honestly don't know if we need the first and last shiftOut cycles
    // Also I feel like we're missing a digitalWrite in the last cycle but whatever it works
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 255);
    digitalWrite(latchPin, HIGH);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, figures[arrayToDisplay][numData]);
    digitalWrite(latchPin, HIGH);

    shiftOut(dataPin, clockPin, MSBFIRST, 255);
    digitalWrite(latchPin, LOW);
}

void dispayError(){
  for(int i = 0; i < 4; i++){
    flashDigit(i+1);
    // Copied the guts of sendRenderToShiftRegister() to make sure this works.
    // I can alter my functions to work with different arrays later
    sendRenderToShiftRegister(errorInFiguresIndex, i);
  }
}

void displayInt(int integer){
  if (integer > 9999 or integer < 0){
    dispayError();
  } else {
    int thousands = integer / 1000;
    int thousandsRemain = integer % 1000;
    int hundreds = thousandsRemain / 100;
    int hundredsRemain = thousandsRemain % 100;
    int tens = hundredsRemain / 10;
    int ones = hundredsRemain % 10;
    flashDigit(1);
    sendRenderToShiftRegister(numsInFiguresIndex, thousands);
    flashDigit(2);
    sendRenderToShiftRegister(numsInFiguresIndex, hundreds);
    flashDigit(3);
    sendRenderToShiftRegister(decimalNumsInFiguresIndex, tens);
    flashDigit(4);
    sendRenderToShiftRegister(numsInFiguresIndex, ones);
  }
}

void loop() {
  // Something tells me this is a bad idea
  sensorValue = analogRead(ldrPin);
  displayInt(sensorValue);
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