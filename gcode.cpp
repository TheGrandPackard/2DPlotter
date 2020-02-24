#include "gcode.h"

extern Axis xAxis;
extern Axis yAxis;
extern Axis zAxis;

void processGCODE(char *command) {

  char *strings[10];
  char *ptr = NULL;
  byte index = 0;
  
  ptr = strtok(command, " ");  // takes a list of delimiters
  while(ptr != NULL)
  {
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, " ");  // takes a list of delimiters
  }

  if(strcmp(strings[0], COMMAND_G0) == 0) {
      g0(strings, index);
  } else if(strcmp(strings[0], COMMAND_G1) == 0) {
      g0(strings, index);
  } else if(strcmp(strings[0], COMMAND_G28) == 0) {
      g28(strings, index);
  } else {
    Serial.print("Unrecognized GCODE Command: ");
    Serial.println(command);
  }
}

// G0 Xnnn Ynnn Znnn Ennn Fnnn Snnn
// G0 X12   ; Move to 12mm on the X axis 
// G1 X90.6 Y13.8 ; Move to 90.6mm on the X axis and 13.8mm on the Y axis 
void g0(char **argv , int argc) {
//  for(int i=0; i<argc; i++) {
//    Serial.print("Argument #");
//    Serial.print(i);
//    Serial.print(": ");
//    Serial.println(argv[i]);
//  }
  
  if(argc < 2){
    Serial.print("Not enough arguments for G0: ");
    Serial.print(argc);
    return;
  }

  // TODO: Make this work for multiple axis parameters

  char *param1 = argv[1];
  char axis = param1[0];
  String param1String = String(param1);
  float distance = param1String.substring(1).toFloat();
  
//  Serial.print("Linear move on axis: ");
//  Serial.print(axis);
//  Serial.print(" Distance: ");
//  Serial.println(distance);

  if(axis == X_AXIS) {
      moveAxis(&xAxis, distance);
  } else if(axis == Y_AXIS) {
      moveAxis(&yAxis, distance);
  } else if(axis == Z_AXIS) {
      moveAxis(&zAxis, distance);
  }
}

// G28     ; Home all axes
// G28 X Z ; Home the X and Z axes
void g28(char **argv , int argc) {
  if(argc==1) {
//    Serial.print("Homing all axes: ");
    homeAxis(&xAxis);
    homeAxis(&yAxis);
    homeAxis(&zAxis);
    return;
  }

  for(int i=1; i < argc; i++) {
    char axis = argv[i][0];
  
     if(axis == X_AXIS) {
//        Serial.println("Homing X axis");
        homeAxis(&xAxis);
      } else if(axis == Y_AXIS) {
//        Serial.println("Homing Y axis");
        homeAxis(&yAxis);
      } else if(axis == Z_AXIS) {
//        Serial.println("Homing Z axis");
        homeAxis(&zAxis);
      }
  } 
}
