#include "stepper.h"
#include "gcode.h"

void setupAxis(Axis *axis) {
  pinMode(axis->StepPin,OUTPUT);
  pinMode(axis->DirPin,OUTPUT); 
  pinMode(axis->LimitPin,INPUT); 
  axis->is_setup = true;
}

void moveAxis(Axis *axis, float distance) {
  if(!axis->is_setup) return;
  if(distance == 0) return;

  // Make sure we don't go beyond the max of the axis
  if(axis->CurPos + distance > axis->MaxPos) {
    Serial.print("Cannot travel beyond maximum of axis. Current position: ");
    Serial.print(axis->CurPos);
    Serial.print(" Maximum position: ");
    Serial.println(axis->MaxPos);
    return; 
  }

  // Make sure we don't try and go beyond the min of the axis-> Use a home command instead
  if(axis->CurPos + distance < axis->MinPos) {
    Serial.print("Cannot travel beyond minimum of axis. Current position: ");
    Serial.print(axis->CurPos);
    Serial.print(" Maximum position: ");
    Serial.println(axis->MaxPos);
    return;
  }
  
//  Serial.print("Moving ");
//  Serial.print(axis->Name);
//  Serial.print(" Axis: ");
//  Serial.print(distance);
//  Serial.println("mm");

  // Update current x with new end position
  axis->CurPos+= distance;
  
  if(distance > 0) {
    digitalWrite(axis->DirPin,HIGH); // Enables the motor to move in a particular direction    
  } else {
    digitalWrite(axis->DirPin,LOW); //Changes the rotations direction
  }
  
  for(int i = 0; i < abs(distance) * pulsesPerMM; i++) {
    // Make sure we don't go negative if the limit switch is engaged
    if (!digitalRead(axis->LimitPin) && distance < 0) {
      Serial.print("Stopped moving axis because of limit switch: ");
      Serial.println(axis->Name);
      break;
    } 
    
    digitalWrite(axis->StepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(axis->StepPin,LOW); 
    delayMicroseconds(500); 
  }
}

void homeAxis(Axis *axis) {
  if(!axis->is_setup) return;
  
//  Serial.print("Homing ");
//  Serial.print(axis->Name);
//  Serial.println(" axis");

  // Move in the negative axis direction
  digitalWrite(axis->DirPin,LOW); //Changes the rotations direction
    
  // Move the axis until the limit switch is engaged
  while(true) {
    if (!digitalRead(axis->LimitPin) ) {
      break;
    } 
    
    digitalWrite(axis->StepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(axis->StepPin,LOW); 
    delayMicroseconds(500); 
  }

  axis->CurPos = 0;
}
