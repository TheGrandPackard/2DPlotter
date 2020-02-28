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
//    for(int i=0; i<argc; i++) {
//      Serial.print("Argument #");
//      Serial.print(i);
//      Serial.print(": ");
//      Serial.println(argv[i]);
//  }

  if(argc < 2){
    Serial.print("Not enough arguments for G0: ");
    Serial.print(argc);
    return;
  }

  // Parse axis parameters
  Axis *x = NULL, *y = NULL;
  float xDistance = 0, yDistance = 0;
  
  for(int i=1; i < argc; i++) {
    char *param = argv[i];
    char axis = param[0];
    String paramString = String(param);
    float distance = paramString.substring(1).toFloat();

    if(axis == X_AXIS) {
      Serial.print("Parsed X Axis: ");
      Serial.println(distance);
      x = &xAxis;
      xDistance = distance;
    } else if(axis == Y_AXIS) {
      Serial.print("Parsed Y Axis: ");
      Serial.println(distance);
      y = &yAxis;
      yDistance = distance;
    }   
  }

  if(x != NULL && y != NULL) {
    Serial.print("Linear move on XY axis: (");
    Serial.print(xDistance);
    Serial.print(", ");
    Serial.print(yDistance);
    Serial.println(")");
    moveAxes(x, xDistance, y, yDistance);
  } else if(x != NULL) {
    Serial.print("Linear move on X axis: ");
    Serial.println(xDistance);
    moveAxis(x, xDistance);
  } else if(y != NULL) {
    Serial.print("Linear move on Y axis: ");
    Serial.println(yDistance);
    moveAxis(y, yDistance);
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
