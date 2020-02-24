#include "gcode.h"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

Axis xAxis = {X_AXIS, 3, 4, 5, 0, 325, 0};
Axis yAxis = {Y_AXIS, 6, 7, 8, 0, 325, 0};
Axis zAxis = {Z_AXIS, 9, 10, 11, 0, 0, 0};

void setup() {
  // setup serial
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // setup the axes
  setupAxis(&xAxis);
  setupAxis(&yAxis);
//  setupAxis(&zAxis);
  
  Serial.println("Packard Plotter Initialized");
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
//    Serial.print("Received GCODE Command: ");
//    Serial.println(inputString);
    
    // Length (with one extra character for the null terminator)
    int str_len = inputString.length() + 1; 
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    // Copy it over 
    inputString.toCharArray(char_array, str_len);
    // Process code
    processGCODE(char_array);
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
