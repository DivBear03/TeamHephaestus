#include "MOVIShield.h"     // Include MOVI library, needs to be *before* the next #include
#include "Servo.h"
//#include "Stepper.h"
//#include <ESP8266WiFi.h>
//#include <WiFiNINA.h>
//#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_PIC32)
#include <SoftwareSerial.h> // This is nice and flexible but only supported on AVR and PIC32 architecture, other boards need to use Serial1 
//#endif

#define echoPin 3 // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D2 Arduino to pin Trig of HC-SR04
#define STEPS 8

//WiFiServer server(80);
//MDNSResponder mdns;

String firstNumber;
String secondNumber;
String thirdNumber;
String fourthNumber;
//int status = WL_IDLE_STATUS;

//WiFiClient client;

MOVI recognizer(true);

bool wait, speaking, standby;
//const int led = 13;
//MOVI recognizer(true);            // Get a MOVI object, true enables serial monitor interface, rx and tx can be passed as parameters for alternate communication pins on AVR architecture

String hardware[16] = {"em five bolt", "em five nut", "em six bolt", "em six nut", "em eight bolt", "em eight nut", "em ten bolt", "em ten nut", "em twelve bolt", "em twelve nut", "em sixteen bolt", "em sixteen nut", "em twentee bolt", "em twentee nut", "em twenty four bolt", "em twenty four nut"};

//double positions[8];
Servo pusher;
Servo rotator;
//Stepper rotator(STEPS, 8, 9, 10, 11);

double stepperPos[4] = {1000, 1150, 1300, 1450};
double stepperResult;
String positions[8] = {"22.5", "67.5", "112.5", "157.5", "one", "two", "three", "four"};


int prevPos;
signed int res;

void setup()
{
  pusher.attach(3);
  pusher.writeMicroseconds(1200);
  rotator.attach(5);
  rotator.writeMicroseconds(1200);
  
  Serial.begin(9600);
  Serial.println("Program working");
  recognizer.init();      // Initialize MOVI (waits for it to boot)

  recognizer.callSign("Gerald"); // Train callsign Arduino (may take 20 seconds)

  recognizer.addSentence("Correct");
  recognizer.addSentence("Incorrect");
//  recognizer.addSentence("Detonate");
//  recognizer.addSentence("Go fuck yourself");
//  recognizer.addSentence("Ready begin");
//  recognizer.addSentence("hello");
  recognizer.addSentence("em five bolt");
  recognizer.addSentence("em six bolt");
  recognizer.addSentence("em eight bolt");
  recognizer.addSentence("em ten bolt");

  recognizer.setThreshold(15);

  recognizer.train();                           // Train (may take 20seconds)
}

  void loop() {
    res = recognizer.poll();
    if (res != 0) {
      Serial.println(res);
      if (res > 2 && res < 7) {
        // if is a recognized event
        stepperResult = stepperPos[res - 3];
        Serial.print("Motor target: ");
        Serial.println(stepperResult);
        recognizer.say("Position " + positions[res+1] + " selected. Confirm selection please");
        res = recognizer.poll();
        while (res != 1 && res != 2) {
          res = recognizer.poll();
        } 
        if (res == 1) {
          recognizer.say("okay, selection confirmed");
          rotator.writeMicroseconds(stepperResult);
          delay(1000);
          pusher.writeMicroseconds(1100); //  just above lowest position of ~900 or something
          delay(1000);
          pusher.writeMicroseconds(1000); // retract
        } else {
          recognizer.say("okay, canceling selection");
        }
      }
    }
  }

void resetServos() {

  pusher.writeMicroseconds(0);
  rotator.writeMicroseconds(0);
  
}
