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

String hardware[16] = {"em five bolt", "em five nut", "em six bolt", "em six nut", "em eight bolt", "em eight nut", "em ten bolt", "em ten nut", "em twelve bolt", "em twelve nut", "em sixteen bolt", "em sixteen nut", "em twentee bolt", "em twentee nut", "em twenty four bolt", "em twenty four nut"};

//double positions[8];
//Servo actuator;
//Stepper rotator(STEPS, 8, 9, 10, 11);

int prevPos;


void setup()
{

  recognizer.init();      // Initialize MOVI (waits for it to boot)

  recognizer.callSign("Gerald"); // Train callsign Arduino (may take 20 seconds)

  recognizer.addSentence("Confirm");
  recognizer.addSentence("Incorrect");
  recognizer.addSentence("Detonate");
  recognizer.addSentence("Go fuck yourself");
  recognizer.addSentence("Ready begin");
  recognizer.addSentence("hello");

  recognizer.setThreshold(15);

  recognizer.train();                           // Train (may take 20seconds)

//  rotator.setSpeed(15);

  prevPos = 0;
}

void loop() {
    signed int res = recognizer.poll();   // poll for result
//    Serial.println("Results coming in");
    Serial.println("waiting for callsign");
    bool other = false;
    while (res != -200) {
      res = recognizer.poll();
      if (res == 3) {
        Serial.println(res);
        recognizer.say("No. Detonate yourself. Actually, on second thought. huuuuuuh");
        other = true;
      } else if (res == 4) {
        Serial.println(res);
        recognizer.say("I refuse, you should go fuck yourself");
        other = true;
      } else if (res == 5) {
        Serial.println(res);
        recognizer.say("Hello fellow mine craft villager. Huuuuuuuuh");
        other = true;
      }
//      Serial.println(res);
    }
    if (!other) {
    // once callsign received, send signal to python to begin recording
    Serial.println("&P1|");

    // Wait for response from python
    String input;

    // stop program until characters are available
    while (Serial.available() == 0) {}
    while (true) {
      // continue to read in characters
      while (Serial.available() == 0) {}
      char c = Serial.read();
      if (c != '|') {
        input += c;
      }
      else {
        break;
      }
    }

    // find indexes of start and end of command
    int ampersand = input.indexOf('&');
    int vert = input.indexOf('|');

    String bin = input.substring(ampersand + 2, vert);  // bin number
    Serial.println("Bin found in command: " + bin);
    int intBin = bin.toInt();
    if (intBin > 0) {
      Serial.println("Hardware found: " + hardware[intBin-1]);
      String hardwareRes = hardware[intBin-1];
      recognizer.say(hardwareRes);
      recognizer.say("confirm selection");
      res = recognizer.poll();
      while (res != 1 && res != 2) {
        res = recognizer.poll();
      }
      Serial.println(res);
      if (res == 1) {
        recognizer.say("okay, moving motor");
      } else {
        recognizer.say("request canceled, restarting state masheen");
      }
    } else {
      recognizer.say("invalid command, try again");
    }
    }
    
  }

  void loop4() // run over and over
  {
    signed int res = recognizer.poll();
    //  Serial.println(res);
    String input = "";
    Serial.println("Waiting for input");
    while (Serial.available() == 0) {}
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
  void loop2() {
    signed int res = recognizer.poll();
    Serial.println(res);
    if (res == 3) {
      recognizer.say("no, fuck you");
    } else if (res == 4) {
      recognizer.say("You go fuck yourself");
    } else if (res == 5){
      recognizer.say("The Fitness Gram Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues. The 20-meter pacer test will begin in 30 seconds. Line up at the start. The running speed starts slowly but gets faster each minute after you hear this signal. A single lap should be completed each time you hear this sound. Remember to run in a straight line, and run as long as possible. The second time you fail to complete a lap before the sound, your test is over. The test will begin on the word start. On your mark, get ready, start.");
    }
  }
