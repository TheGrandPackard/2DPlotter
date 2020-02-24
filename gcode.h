#include "stepper.h"
#include "hardwareSerial.h"

#ifndef GCODE_H
#define GCODE_H

const char X_AXIS = 'X';
const char Y_AXIS = 'Y';
const char Z_AXIS = 'Z';

#define COMMAND_G0 "G0"
#define COMMAND_G1 "G1"
#define COMMAND_G28 "G28"

// https://reprap.org/wiki/G-code
// https://marlinfw.org/docs/gcode/G000-G001.html

void processGCODE(char *command);

void g0(char **argv , int argc);
void g28(char **argv , int argc);

#endif
