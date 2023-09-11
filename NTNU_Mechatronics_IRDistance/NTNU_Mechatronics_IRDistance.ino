#define IRdistancePin 3

void setup()  {
    Serial.begin(9600);
    pinMode(IRdistancePin,INPUT);
}
void loop()  {
        delay(500);
        if(digitalRead(IRdistancePin)==LOW)  {
            Serial.println("Can/Bottle detected");
        }
}
