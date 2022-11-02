#include "MOVIShield.h"     // Include MOVI library, needs to be *before* the next #include
MOVI recognizer(true);
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_PIC32)
#include <SoftwareSerial.h> // This is nice and flexible but only supported on AVR and PIC32 architecture, other boards need to use Serial1 
#endif

const int led = 13;
//MOVI recognizer(true);            // Get a MOVI object, true enables serial monitor interface, rx and tx can be passed as parameters for alternate communication pins on AVR architecture

void setup()  
{
  pinMode(led, OUTPUT);    // Make LED port writeable
  digitalWrite(led, HIGH); // Blink LED.   
  delay(1);                   
  digitalWrite(led, LOW);  
  delay(1); 

  recognizer.init();      // Initialize MOVI (waits for it to boot)

  //*
  // Note: training can only be performed in setup(). 
  // The training functions are "lazy" and only do something if there are changes. 
  // They can be commented out to save memory and startup time once training has been performed.
  recognizer.callSign("Oliver"); // Train callsign Arduino (may take 20 seconds)
  recognizer.addSentence("Turn on"); // Add sentence 1
  recognizer.addSentence("Turn off");            // Add sentence 2
  recognizer.train();                           // Train (may take 20seconds) 
  //*/

  //  recognizer.setThreshold(5);      // uncomment and set to a higher value (valid range 2-95) if you have a problems due to a noisy environment.
}

void loop() // run over and over
{
  signed int res=recognizer.poll(); // Get result from MOVI, 0 denotes nothing happened, negative values denote events (see docs)
  if (res==1) {                     // Sentence 1.
    digitalWrite(led, HIGH);        // Turn on LED
    recognizer.say("and there was light!"); // Speak a sentence
  } 
  if (res==2) {                    // Sentence 2 
    digitalWrite(led, LOW);        // Turn off LED
  }
  // Do more ...
}
