#include "MOVIShield.h"     // Include MOVI library, needs to be *before* the next #include
#include "Servo.h"
#include "Stepper.h"
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_PIC32)
#include <SoftwareSerial.h> // This is nice and flexible but only supported on AVR and PIC32 architecture, other boards need to use Serial1 
#endif

#define STEPS 8

MOVI recognizer(true);

bool wait, speaking, standby;
const int led = 13;
//MOVI recognizer(true);            // Get a MOVI object, true enables serial monitor interface, rx and tx can be passed as parameters for alternate communication pins on AVR architecture

String hardware[16];
double positions[8];
Servo actuator;
Stepper rotator(STEPS, 8, 9, 10, 11);

int prevPos;


void setup()  
{ 
  
  recognizer.init();      // Initialize MOVI (waits for it to boot)
  
  recognizer.callSign("Gerald"); // Train callsign Arduino (may take 20 seconds)
  
  recognizer.addSentence("Confirm");
  recognizer.addSentence("Incorrect");
  
  recognizer.train();                           // Train (may take 20seconds) 

  rotator.setSpeed(30);

  prevPos = 0;  

  hardware = ["emfive bolt", "emfive nut", "emsix bolt", "emsix nut", "em eight bolt", "em eight nut", 
  "emten bolt", "emten nut", "em twelve bolt", "em twelve nut", "em sixteen bolt", "em sixteen nut", "em twentee bolt", 
  "em twentee nut", "em twenty four bolt", "em twenty four nut", "em thirtee bolt", "em thirtee nut"];

void loop() {
  signed int res = recognizer.poll();   // poll for result
  
  // waiting for callsign
  while(res != -200) {res = recognizer.poll();}
  
  // once callsign received, send signal to python to begin recording
  Serial.println("&P1|");
  
  // Wait for response from python
  String input;

  // stop program until characters are available
  while (Serial.available() == 0){}
  while (true) {
    // continue to read in characters
    while (Serial.available() == 0) {}
      char c = Serial.read();
      if (c != '|') {input += c;}
      else {break;}
  }

  // find indexes of start and end of command
  int ampersand = input.indexOf('&');
  int vert = input.indexOf('|');
  
  String bin = input.substring(ampersand+2, vert);    // bin number
  Serial.println("Bin found in command: " + bin);
  int intBin = bin.toInt();
  rotator.step(intBin - prevPos);
}

void loop2() // run over and over
{
  signed int res = recognizer.poll();
//  Serial.println(res);
  String input = "";
  Serial.println("Waiting for input");
  while(Serial.available() == 0) {}
  while (true) {
    while (Serial.available() == 0) {}
    char c = Serial.read();
//    Serial.println(c);
    if (c != '|') {
      input += c;
    } else {
      break;
    }
  }
  Serial.println(input + " Received");
  recognizer.say(input);
  res = recognizer.poll();
  while (res != -151) {
    res = recognizer.poll();
  }
  Serial.println("MOVI has spoken");
  
}
