#include "MOVIShield.h"     // Include MOVI library, needs to be *before* the next #include
#include "Servo.h"
#include "Stepper.h"
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
//Servo actuator;
//Stepper rotator(STEPS, 8, 9, 10, 11);

int prevPos;
signed int res;

void setup()
{
  Serial.begin(9600);
  Serial.println("Program working");
  recognizer.init();      // Initialize MOVI (waits for it to boot)

  recognizer.callSign("Gerald"); // Train callsign Arduino (may take 20 seconds)

  recognizer.addSentence("Confirm");
  recognizer.addSentence("Incorrect");
//  recognizer.addSentence("Detonate");
//  recognizer.addSentence("Go fuck yourself");
//  recognizer.addSentence("Ready begin");
//  recognizer.addSentence("hello");
  recognizer.addSentence("dispense em five bolt");
  recognizer.addSentence("dispense em five nut");
  recognizer.addSentence("dispense em six bolt");
  recognizer.addSentence("dispense em six nut");
  recognizer.addSentence("dispense em eight bolt");
  recognizer.addSentence("dispense em eight nut");
  recognizer.addSentence("dispense em ten bolt");
  recognizer.addSentence("dispense em ten nut"); 
  recognizer.addSentence("dispense em twelve bolt");
  recognizer.addSentence("dispense em twelve nut");
  recognizer.addSentence("dispense em sixteen bolt"); 
  recognizer.addSentence("dispense em sixteen nut"); 
  recognizer.addSentence("dispense em twentee bolt"); 
  recognizer.addSentence("dispense em twentee nut");
  recognizer.addSentence("dispense em twenty four bolt");
  recognizer.addSentence("dispense em twenty four nut");

//  recognizer.addSentence("return em five bolt");
//  recognizer.addSentence("return em five nut");
//  recognizer.addSentence("return em six bolt");
//  recognizer.addSentence("return em six nut");
//  recognizer.addSentence("return em eight bolt");
//  recognizer.addSentence("return em eight nut");
//  recognizer.addSentence("return em ten bolt");
//  recognizer.addSentence("return em ten nut"); 
//  recognizer.addSentence("return em twelve bolt");
//  recognizer.addSentence("return em twelve nut");
//  recognizer.addSentence("return em sixteen bolt"); 
//  recognizer.addSentence("return em sixteen nut"); 
//  recognizer.addSentence("return em twentee bolt"); 
//  recognizer.addSentence("return em twentee nut");
//  recognizer.addSentence("return em twenty four bolt");
//  recognizer.addSentence("return em twenty four nut");

  recognizer.setThreshold(15);

  recognizer.train();                           // Train (may take 20seconds)
}

//void loop1() {
//    signed int res = recognizer.poll();   // poll for result
//    Serial.println("waiting for callsign");
//    bool other = false; // indicator of other command or legitimate command
//    
//    while (res != -200) {
//      res = recognizer.poll();
//      if (res == 3) {
//        Serial.println(res);
//        recognizer.say("No. Detonate yourself. Actually, on second thought. huuuuuuh");
//        other = true;
//        break;
//      } else if (res == 4) {
//        Serial.println(res);
//        recognizer.say("Nah bro. you should go fuck yourself");
//        other = true;
//        break;
//      } else if (res == 5) {
//        Serial.println(res);
//        recognizer.say("Hello fellow mine craft villager. Huuuuuuuuh");
//        other = true;
//        break;
//      }
//    }
//
//    // if a legitimate command
//    if (!other) {
//      // once callsign received, send signal to python to begin recording
//      Serial.println("&P1|");
//  
//      // Wait for response from python
//      String input;
//  
//      // stop program until characters are available
//      while (Serial.available() == 0) {}
//      while (true) {
//        // continue to read in characters
//        while (Serial.available() == 0) {}
//        char c = Serial.read();
//        if (c != '|') {
//          input += c;
//        }
//        else {
//          break;
//        }
//      }
//  
//      // find indexes of start and end of command
//      int ampersand = input.indexOf('&');
//      int vert = input.indexOf('|');
//  
//      String bin = input.substring(ampersand + 2, vert);  // bin number
//      Serial.println("Bin found in command: " + bin);
//      int intBin = bin.toInt();
//      if (intBin > 0) {
//        Serial.println("Hardware found: " + hardware[intBin-1]);
//        String hardwareRes = hardware[intBin-1];
//        recognizer.say(hardwareRes);
//        recognizer.say("confirm selection");
//        res = recognizer.poll();
//        while (res != 1 && res != 2) {
//          res = recognizer.poll();
//        }
//        Serial.println(res);
//        if (res == 1) {
//          recognizer.say("okay, moving motor");
//        } else {
//          recognizer.say("request canceled, restarting state masheen");
//        }
//      } else {
//        recognizer.say("invalid command, try again");
//      }
//    }
//  }
  void loop() {
    res = recognizer.poll();
    if (res != 0) {
      Serial.println(res);
      if (res == 1) {
        recognizer.say("Confirm");
      }else if (res == 2) {
        recognizer.say("Incorrect");
      } else {
        float angle = (res - 3) * 45;
        recognizer.say("moving motor to");
        Serial.print("Motor position: ");
        Serial.println(angle);
      }
    }
  }
