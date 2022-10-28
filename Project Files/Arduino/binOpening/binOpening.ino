#include <Servo.h>

const int arraySize = 4;
String bins[arraySize] = {"Wrenches", "Nuts", "Bolts", "Screws"};
Servo servoArray[arraySize];

void setup() {

  attachServos();

}

void loop () {

    String recievedWord = inputWord();
    Serial.println("Word is: " + recievedWord);

    int binNum = findBinNum(recievedWord);
    if (binNum == -1) {
      Serial.println("Invalid word.");
    } else {
      openBin(binNum);
    }

}

void attachServos() {

  servoArray[0].attach(9);
  servoArray[1].attach(10);
  servoArray[2].attach(11);
  servoArray[3].attach(12);
  
}

String inputWord() {

  Serial.println("Enter data:");
  while (Serial.available() == 0) {}     //wait for data available
  String wordInput = Serial.readString();  //read until timeout
  wordInput.trim();                        // remove any \r \n whitespace at the end of the String
  return wordInput;
  
}

int findBinNum(String recievedWord) {

  boolean found = false;
  for (int i = 0; i < arraySize; i++) {
    if (recievedWord == bins[i]) {
      return i;
    }
  }

  if (!found) {
    return -1;
  }
  
}

void openBin(int binNum) {

  for (int pos = 0; pos < 180; pos++) {
    servoArray[binNum].write(pos);
  }

  for (int pos = 180; pos > 0; pos--) {
    servoArray[binNum].write(pos);
  }
  
}
