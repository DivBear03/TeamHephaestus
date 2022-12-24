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

double stepperPos[4] = {500, 1000, 1450, 2000};
double stepperResult;
String positions[8] = {"22.5", "67.5", "112.5", "157.5", "one", "two", "three", "four"};


int prevPos;
signed int res;

void setup()
{
  pusher.attach(3);
  pusher.writeMicroseconds(1000);
  rotator.attach(5);
  rotator.writeMicroseconds(500);
  
  Serial.begin(9600);
  Serial.println("Program working");
  recognizer.init();      // Initialize MOVI (waits for it to boot)

  recognizer.callSign("Gerald"); // Train callsign Arduino (may take 20 seconds)

  recognizer.addSentence("Correct");
  recognizer.addSentence("Incorrect");

  recognizer.addSentence("servo savers");
  recognizer.addSentence("screws");
  recognizer.addSentence("em three nuts");
  recognizer.addSentence("spacers");

  recognizer.addSentence("Detonate");
  recognizer.addSentence("Go fuck yourself");
  recognizer.addSentence("Ready begin");
  recognizer.addSentence("hello");

  recognizer.setThreshold(15);

  recognizer.train();                           // Train (may take 20seconds)
}

//void loop2() {
//  while (!Serial.available()) {}
//  String command = Serial.readStringUntil("\n");
//  
//  pusher.writeMicroseconds(command.toInt());
//}

  void loop() {
    res = recognizer.poll();
    if (res != 0) {
      if (res > 2 && res < 7) {
        // if is a recognized event
        stepperResult = stepperPos[res - 3];
        recognizer.say("Position three selected");
        delay(2000);
        res = recognizer.poll();
        while (res != 1 && res != 2) {
          res = recognizer.poll();
        } 
        if (res == 1) {
          recognizer.say("okay, selection confirmed");
          rotator.writeMicroseconds(stepperResult);
          delay(1000);
          pusher.writeMicroseconds(1600); //  just above lowest position of ~900 or something
          delay(1000);
          pusher.writeMicroseconds(1000); // retract
        } else {
          recognizer.say("okay, canceling selection");
        }
      } else if (res > 6 && res < 11) {
        if (res == 3) {
          recognizer.say("No. Detonate yourself. Actually, on second thought. huuuuuuh");
        } else if (res == 4) {
          recognizer.say("I refuse, you should go fuck yourself");
        } else if (res == 5) {
          recognizer.say("The Fitness Gram Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues.");
        } else if (res == 5) {
          recognizer.say("Hello fellow mine craft villager. Huuuuuuuuh");
        }
      }
    }
  }
//
//void resetServos() {
//
//  pusher.writeMicroseconds(0);
//  rotator.writeMicroseconds(0);
//  
//}
