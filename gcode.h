#include "WString.h"
#ifndef GCODE_H
#define GCODE_H

// https://reprap.org/wiki/G-code

// G0 X12   ; Move to 12mm on the X axis 
const String G0 = "G0";
void g0();

// G1 X90.6 Y13.8 ; Move to 90.6mm on the X axis and 13.8mm on the Y axis 
const String G1 = "G1";
void g1();

// G28     ; Home all axes
// G28 X Z ; Home the X and Z axes
const String G28 = "28";
void g28();

#endif
