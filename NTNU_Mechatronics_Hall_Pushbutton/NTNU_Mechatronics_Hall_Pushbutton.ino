/*
 * Working simple code for Hall Effect sensor and pushbutton
 * Mechatronics @ NTNU 2023
 */
 
#define hallSensorPin A0    // Hall Effect sensor pin
                            // outside leads to ground and +5V
int hallSensorValue = 0;    // variable to store the value read

#define pushbuttonPin 4     // Pushbutton pin
int pushbuttonState = 0;    // Pushbutton state

void setup() {
  Serial.begin(9600);           //  setup serial
  pinMode(pushbuttonPin, INPUT_PULLUP);  //
}

void loop() {
  hallSensorValue = analogRead(hallSensorPin);  // read the value of Hall Effect sensor 
  pushbuttonState = digitalRead(pushbuttonPin); // read state of pushbutton
  Serial.print("Hall Effect Sensor value: ");
  Serial.print(hallSensorValue);          // print the value
  Serial.print(", Pushbutton state: ");
  Serial.println(pushbuttonState);
  
}
