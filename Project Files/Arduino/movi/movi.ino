#include "MOVIShield.h"
const int rpiout = 13;
MOVI recognizer(true);

void setup() {
//    Serial.begin(9600);
    pinMode(rpiout, OUTPUT);
//    Serial.println("Hello");
    recognizer.init();
    recognizer.callSign("oliver");
    recognizer.addSentence("give me some light");
    recognizer.addSentence("shut down now");
    recognizer.train();
    recognizer.setThreshold(5);
    recognizer.say("Trained");
//    Serial.println("Trained");
}

void loop() {
    signed int res = recognizer.poll();
//    Serial.println("Looping"); 
    // If activation word detected, send signal
    // to R Pi over connection
    if (res  == 1) {
//        digitalWrite(rpiout, HIGH);
        recognizer.say("and there was light");
//        Serial.println("Light");
    } 
    if (res == 2) {
//      digitalWrite(rpiout, LOW);
      recognizer.say("Shutting down");
//      Serial.prin tln("Dark");
    }

//  digitalWrite(rpiout, HIGH);
//  delay(500);
//  digitalWrite(rpiout, LOW);
//  delay(500);
  
}
