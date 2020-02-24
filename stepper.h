#include <Arduino.h>
#include "hardwareSerial.h"

#ifndef STEPPER_H
#define STEPPER_H

// Stepper Motor Notes
// 200 pulses in 1 rotation
// 1 rotation is 40mm travel (12mm diameter gear)
// 5 pulses per mm travel
const int pulsesPerMM = 5;

const String X_AXIS = "X";
const String Y_AXIS = "Y";
const String Z_AXIS = "Z";

typedef struct {
  String Name;
  int StepPin, DirPin, LimitPin;
  float MinPos, MaxPos, CurPos;
} Axis;

void setupAxis(Axis *axis);
void moveAxis(Axis *axis, float distance);
void homeAxis(Axis *axis);

#endif
