/*
   Merged code for Paperbots assignment - project Pant Machine - team nr. 3
   Mechatronics @ NTNU 2023
*/

// include TFT and SPI libraries
#include <TFT.h>
#include <SPI.h>


#define SPEAKER 3           // speaker pin
#define hallSensorPin A0    // Hall Effect sensor pin, left and middle leads to ground and +5V
#define pushbuttonPin 4     // Pushbutton pin

// TFT display pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8

int hallSensorValue = 0;    // variable to store the value read
int pushbuttonState = 0;    // Pushbutton state

int BassTab[] = {1911, 1702, 1516, 1431, 1275, 1136, 1012}; //bass 1~7

int arduinoState = 0; // state of arduino - 0 = start; 1 = can inserted; 2 = waiting for pushbutton press; 3 = lottery check


// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode(pushbuttonPin, INPUT_PULLUP);  // setup pushbutton
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(SPEAKER, LOW);

  TFTscreen.begin();  //initialize the TFT library

  TFTscreen.background(0, 0, 0);  // clear the screen with a black background
  //generate a random color
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);

  // set a random font color
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);

  TFTscreen.setTextSize(3);   //set the text size
  TFTscreen.text("Booting!", 6, 57);
}

void loop() {
  if (arduinoState == 0){         // start, waiting for can to be insterted
    hallSensorValue = analogRead(hallSensorPin);  // read the value of Hall Effect sensor
    TFTscreen.text("Insert can.", 6, 57);
    if (hallSensorValue == 1){  // can insterted?
      arduinoState = 1; // go to next state
    }
  }
  else if (arduinoState == 1){    // can insterted, play speaker sound
    TFTscreen.text("Can inserted.", 6, 57);
    speakerPlay();
    arduinoState = 2; // go to next state
    delay(1000);
  }
  else if (arduinoState == 2){    // sound played, press pushbutton to check lottery win
    pushbuttonState = digitalRead(pushbuttonPin); // read state of pushbutton
    TFTscreen.text("Press button to check win", 6, 57);
    if (pushbuttonState == 1){
      arduinoState = 3;
    }
  }
  else if (arduinoState == 3){
    Serial.println("Pushbutton pressed, check lottery");
    // some lottery function, set something to the display
    TFTscreen.text("Checking win!", 6, 57);
    delay(1000);
    if (random(0,1) == 0){
      TFTscreen.text("No win today :-(", 6, 57);
    }
    else {
      TFTscreen.text("You've won! CG!", 6, 57);
    }
    delay(3000);
    arduinoState = 0; // set arduinoState to start this flow again
  }
  
  /*
   * and now what? What are we going to do with the inserted can? 
   * Maybe there can be a servo and after the lottery check the servo can "flush" the can to the back of our pant machine
   */
}

void speakerPlay(){
    /*sound bass 1~7*/
  for (int note_index = 0; note_index < 7; note_index++)
  {
    sound(note_index);
    delay(500);
  }
}

void sound(uint8_t note_index)  // function for speaker
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(SPEAKER, HIGH);
    delayMicroseconds(BassTab[note_index]);
    digitalWrite(SPEAKER, LOW);
    delayMicroseconds(BassTab[note_index]);
  }
}
