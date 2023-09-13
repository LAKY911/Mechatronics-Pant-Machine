/*
   Merged code for Paperbots assignment - project Pant Machine - team nr. 3
   Mechatronics @ NTNU 2023
*/

// include TFT and SPI libraries
#include <TFT.h>
#include <SPI.h>
#include "pitches.h" //Include notes


#define SPEAKER 3           // speaker pin
#define IRdistancePin 6    // IR Distance pin
#define pushbuttonPin 7     // Pushbutton pin

// TFT display pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8

int IRDistanceValue = 0;    // variable to store the value read
int pushbuttonState = 0;    // Pushbutton state

int BassTab[] = {1911, 1702, 1516, 1431, 1275, 1136, 1012}; //bass 1~7

int arduinoState = 0; // state of arduino - 0 = start; 1 = can inserted; 2 = waiting for pushbutton press; 3 = lottery check
int randomNumberWin = 0;



int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int loseMelody[] = {
    NOTE_A3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_C3, NOTE_A2
};

//Global counter for number of cans
int numCans = 0;

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode(pushbuttonPin, INPUT_PULLUP);  // setup pushbutton
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(SPEAKER, LOW);
  pinMode(IRdistancePin,INPUT);

  randomSeed(analogRead(0)); // setup for the randomiser

  TFTscreen.begin();  //initialize the TFT library

  TFTscreen.background(0, 0, 0);  // clear the screen with a black background
  //generate a random color
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);

  // set a random font color
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);

  TFTscreen.setTextSize(2);   //set the text size  
  TFTscreen.background(0,0,0); 
  TFTscreen.text("Insert can.", 6, 57);
}

void loop() {
  if (arduinoState == 0){         // start, waiting for can to be insterted
    IRDistanceValue = digitalRead(IRdistancePin);  // read the value of Hall Effect sensor
    if (IRDistanceValue == LOW){  // can insterted?
      arduinoState = 1; // go to next state
      TFTscreen.background(0,0,0); 
      TFTscreen.text("Can inserted.", 6, 57);
      delay(500);
    }
    else if (digitalRead(pushbuttonPin) == 0){    // sound played, press pushbutton to check lottery win
    arduinoState = 3;
    }
  }

  else if (arduinoState == 1){    // can inserted, play speaker sound
    speakerPlay();
    incrementDisplay();
    arduinoState = 0;
  }
  
  else if (arduinoState == 3){
    Serial.println("Pushbutton pressed, checking lottery");
    numCans = 0; //Reset number of cans
    arduinoState = 0; //Reset state
    // some lottery function, set something to the display
    delay(200);
    TFTscreen.background(0,0,0); 
    TFTscreen.text("Checking win!", 6, 57);
    delay(1000);
    randomNumberWin = random(100);
    if (randomNumberWin < 50){
      Serial.println(randomNumberWin);
      TFTscreen.background(0,0,0); 
      TFTscreen.text("No win today :-(", 6, 57);
      for (int i = 0; i < 6; i++) {
        tone(SPEAKER, loseMelody[i], 200);
        delay(250);  // Add a short pause between notes
      }
    }
    else {
      Serial.println(randomNumberWin);
      TFTscreen.background(0,0,0);
      TFTscreen.text("You've won!", 6, 57);
      delay(1000);
      TFTscreen.background(0,0,0);
      for (int thisNote = 0; thisNote < 8; thisNote++) {
        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(SPEAKER, melody[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(SPEAKER);
      }
      for (int i=0;  i<3; i++){
        // Draw the firework
        int x = random(TFTscreen.width());
        int y = random(TFTscreen.height());
        uint16_t color = 0xFFFF00;
        drawFirework(x, y, color);
        delay(random(200, 1000)); // Delay between fireworks
      }
    }
    delay(3000);
    arduinoState = 0; // set arduinoState to start this flow again
    TFTscreen.background(0,0,0); 
    TFTscreen.text("Feed me cans!", 6, 57);
  }
  
  /*
   * and now what? What are we going to do with the inserted can? 
   * Maybe there can be a servo and after the lottery check the servo can "flush" the can to the back of our pant machine
   */
}

void incrementDisplay(){
  numCans++;
  TFTscreen.background(0,0,0);
  char str[12];
  sprintf(str, "%d", numCans);
  TFTscreen.text(str, 6, 57);
}


void speakerPlay(){
    /*sound bass 1~7*/
    for (int note_index = 0; note_index < 2; note_index++)
  {
    sound(note_index);
    delay(200);
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

void drawFirework(int x, int y, uint16_t color) {
  int numParticles = 50; // Number of particles in the firework

  for (int i = 0; i < numParticles; i++) {
    int angle = random(360);
    float velocity = random(30,40);
    int x1 = x;
    int y1 = y;

    while (x1 >= 0 && x1 < TFTscreen.width() && y1 >= 0 && y1 < TFTscreen.height()) {
      // Calculate the next position
      x1 += velocity * cos(angle * PI / 180);
      y1 += velocity * sin(angle * PI / 180);

      // Draw the particle
      TFTscreen.drawPixel(x1, y1, color);
      TFTscreen.drawPixel(-x1, -y1, color);
      // Fade the particle color
      color -= 0x0001;

      // Delay to control the particle speed
      delay(5);
    }
  }
}
