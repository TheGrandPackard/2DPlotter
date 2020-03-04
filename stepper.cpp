#include "stepper.h"
#include "gcode.h"

void setupAxis(Axis *axis) {
  pinMode(axis->StepPin,OUTPUT);
  pinMode(axis->DirPin,OUTPUT); 
  pinMode(axis->LimitPin,INPUT); 
  axis->is_setup = true;
}

bool axisSafetyCheck(Axis *axis, float distance) {
  // Make sure we don't go beyond the max of the axis
  if(axis->CurPos + distance > axis->MaxPos) {
    Serial.print("Cannot travel beyond maximum of axis: ");
    Serial.print(axis->Name);
    Serial.print(" Current position: ");
    Serial.print(axis->CurPos);
    Serial.print(" Maximum position: ");
    Serial.print(axis->MaxPos);
    Serial.print("\n");
    return false; 
  }

  // Make sure we don't try and go beyond the min of the axis-> Use a home command instead
  if(axis->CurPos + distance < axis->MinPos) {
    Serial.print("Cannot travel beyond minimum of axis: ");
    Serial.print(axis->Name);
    Serial.print(" Current position: ");
    Serial.print(axis->CurPos);
    Serial.print(" Maximum position: ");
    Serial.print(axis->MaxPos);
    Serial.print("\n");
    return false;
  }
  
  return true;
}

void moveAxis(Axis *axis, float distance) {
  if(!axis->is_setup) return;
  if(distance == 0) return;
  if(!axisSafetyCheck(axis, distance)) return;
  
//  Serial.print("Moving ");
//  Serial.print(axis->Name);
//  Serial.print(" Axis: ");
//  Serial.print(distance);
//  Serial.print("mm");
//  Serial.print("\n");

  // Update current x with new end position
  axis->CurPos+= distance;
  
  if(distance > 0) {
    digitalWrite(axis->DirPin,HIGH); // Enables the motor to move in a particular direction    
  } else {
    digitalWrite(axis->DirPin,LOW); //Changes the rotations direction
  }
  
  for(float i = 0; i < abs(distance); i += 1/pulsesPerMM) {
    // Make sure we don't go negative if the limit switch is engaged
    if (!digitalRead(axis->LimitPin) && distance < 0) {
      Serial.print("Stopped moving axis because of limit switch: ");
      Serial.print(axis->Name);
      Serial.print("\n");
      break;
    } 
    
    digitalWrite(axis->StepPin,HIGH); 
    delayMicroseconds(pulseDelay); 
    digitalWrite(axis->StepPin,LOW); 
    delayMicroseconds(pulseDelay); 
  }
}

void moveAxes(Axis *xAxis, float xDistance, Axis *yAxis, float yDistance) {
  if(!xAxis->is_setup || !yAxis->is_setup) return;
  if(xDistance == 0 || yDistance == 0) return;
  if(!axisSafetyCheck(xAxis, xDistance)) return;
  if(!axisSafetyCheck(yAxis, yDistance)) return;
  
//  Serial.print("Moving X Axis: ");
//  Serial.print(xDistance);
//  Serial.print(" Y Axis: ");
//  Serial.print(yDistance);
//  Serial.print("\n");

  // Update current x and y with new end position
  xAxis->CurPos+= xDistance;
  yAxis->CurPos+= yDistance;
  
  if(xDistance > 0) {
    digitalWrite(xAxis->DirPin,HIGH); // Enables the motor to move in a particular direction    
  } else {
    digitalWrite(xAxis->DirPin,LOW); //Changes the rotations direction
  }
  
  if(yDistance > 0) {
    digitalWrite(yAxis->DirPin,HIGH); // Enables the motor to move in a particular direction    
  } else {
    digitalWrite(yAxis->DirPin,LOW); //Changes the rotations direction
  }

  // Find larger axis distance (abs)
  // Find ratio between axis
  Axis *largerAxis = NULL;
  Axis *smallerAxis = NULL;
  float largerDistance = 0;
  float axisRatio = 0;
  if (abs(xDistance) > abs(yDistance)) {
    largerAxis = xAxis;
    smallerAxis = yAxis;
    largerDistance = xDistance;
    axisRatio = abs(xDistance) / abs(yDistance);
  } else {
    largerAxis = yAxis;
    smallerAxis = xAxis;
    largerDistance = yDistance;
    axisRatio = abs(yDistance) / abs(xDistance);
  }

  float smallerAxisCounter = 0;

  // Move larger axis with smaller axis at ratio intervals
  for(float i = 0; i < abs(largerDistance); i += 1/pulsesPerMM) {
    // Make sure we don't go negative if the limit switch is engaged
    if (!digitalRead(xAxis->LimitPin) && xDistance < 0) {
      Serial.print("Stopped moving because of X axis limit switch\n");
      break;
    } else if (!digitalRead(yAxis->LimitPin) && yDistance < 0) {
      Serial.print("Stopped moving because of Y axis limit switch\n");
      break;
    }

    // Always move larger axis
    digitalWrite(largerAxis->StepPin,HIGH); 
    delayMicroseconds(pulseDelay); 
    digitalWrite(largerAxis->StepPin,LOW); 
    
    // Move smaller axis if applicable
    smallerAxisCounter += 1/pulsesPerMM;
    if (smallerAxisCounter >= axisRatio) {
      delayMicroseconds(pulseDelay); 
      digitalWrite(smallerAxis->StepPin,HIGH); 
      delayMicroseconds(pulseDelay);
      digitalWrite(smallerAxis->StepPin,LOW); 
      smallerAxisCounter - axisRatio;
    } else {
      delayMicroseconds(pulseDelay);
    }
  }
}

void homeAxis(Axis *axis) {
  if(!axis->is_setup) return;
  
//  Serial.print("Homing ");
//  Serial.print(axis->Name);
//  Serial.print(" axis");
//  Serial.print("\n");

  // Move in the negative axis direction
  digitalWrite(axis->DirPin,LOW); //Changes the rotations direction
    
  // Move the axis until the limit switch is engaged
  while(true) {
    if (!digitalRead(axis->LimitPin) ) {
      break;
    } 
    
    digitalWrite(axis->StepPin,HIGH); 
    delayMicroseconds(pulseDelay); 
    digitalWrite(axis->StepPin,LOW); 
    delayMicroseconds(pulseDelay); 
  }

  axis->CurPos = 0;
}
