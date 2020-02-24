#include "stepper.h"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

Axis xAxis = {X_AXIS, 3, 4, 5, 0, 325, 0};
Axis yAxis = {Y_AXIS, 6, 7, 8, 0, 325, 0};    

void setup() {
  // setup serial
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // setup the axes
  setupAxis(&xAxis);
  setupAxis(&yAxis);
  
  Serial.println("Packard Plotter Initialized");
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

    // Parse the GCODE command and execute it
    float distance = inputString.toFloat();
    moveAxis(&xAxis, distance);
//    homeAxis(&xAxis);
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } else { 
      // add it to the inputString:
      inputString += inChar;
    }
  }
}
