/*
 * Working simple code for TFT display
 * Mechatronics @ NTNU 2023
 * 
 ***************************************************
 * Web tutorial: https://randomnerdtutorials.com/guide-to-1-8-tft-display-with-arduino/
 * 
 * 1.8 TFT Display Wiring to Arduino Uno
 * TFT display pin | Arduino Uno pin
 * LED | 3.3 V
 * SCK | 13
 * SDA | 11
 * A0 or DC | 9
 * RESET | 8
 * CS | 10
 * GND | GND
 * VCC | 5 V
 */


// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>

// pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8


// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {

  //initialize the library
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  //set the text size
  TFTscreen.setTextSize(2);
}

void loop() {

  //generate a random color
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);
  
  // set a random font color
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);
  
  // print Hello, World! in the middle of the screen
  TFTscreen.text("Hello, World!", 6, 57);
  
  // wait 200 miliseconds until change to next color
  delay(200);
}
