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
    Serial.print("echo:Unknown command: \"");
    Serial.print(command);
    Serial.print("\n");
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
//      Serial.print(argv[i]);
//      Serial.print("\n");
//  }

  if(argc < 2){
    Serial.print("Not enough arguments for G0: ");
    Serial.print(argc);
    Serial.print("\n");
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
//      Serial.print("Parsed X Axis: ");
//      Serial.print(distance);
//      Serial.print("\n");
      x = &xAxis;
      xDistance = distance;
    } else if(axis == Y_AXIS) {
//      Serial.print("Parsed Y Axis: ");
//      Serial.print(distance);
//      Serial.print("\n");
      y = &yAxis;
      yDistance = distance;
    }   
  }

  if(x != NULL && y != NULL) {
//    Serial.print("Linear move on XY axis: (");
//    Serial.print(xDistance);
//    Serial.print(", ");
//    Serial.print(yDistance);
//    Serial.print(")");
    Serial.print("\n");
    moveAxes(x, xDistance, y, yDistance);
  } else if(x != NULL) {
//    Serial.print("Linear move on X axis: ");
//    Serial.print(xDistance);
//    Serial.print("\n");
    moveAxis(x, xDistance);
  } else if(y != NULL) {
//    Serial.print("Linear move on Y axis: ");
//    Serial.print(yDistance);
//    Serial.print("\n");
    moveAxis(y, yDistance);
  }
  
  Serial.print("ok\n");
}

// G28     ; Home all axes
// G28 X Z ; Home the X and Z axes
void g28(char **argv , int argc) {
  if(argc==1) {
//    Serial.print("Homing all axes: ");
    homeAxis(&xAxis);
    homeAxis(&yAxis);
    homeAxis(&zAxis);
  } else {
    for(int i=1; i < argc; i++) {
      char axis = argv[i][0];
    
       if(axis == X_AXIS) {
  //        Serial.print("Homing X axis\n");
          homeAxis(&xAxis);
        } else if(axis == Y_AXIS) {
  //        Serial.print("Homing Y axis\n");
          homeAxis(&yAxis);
        } else if(axis == Z_AXIS) {
  //        Serial.print("Homing Z axis\n");
          homeAxis(&zAxis);
        }
    }
  } 

  Serial.print("ok\n");
}
