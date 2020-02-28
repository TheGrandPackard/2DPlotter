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
    Serial.println(axis->MaxPos);
    return false; 
  }

  // Make sure we don't try and go beyond the min of the axis-> Use a home command instead
  if(axis->CurPos + distance < axis->MinPos) {
    Serial.print("Cannot travel beyond minimum of axis: ");
    Serial.print(axis->Name);
    Serial.print(" Current position: ");
    Serial.print(axis->CurPos);
    Serial.print(" Maximum position: ");
    Serial.println(axis->MaxPos);
    return false;
  }
  
  return true;
}

void moveAxis(Axis *axis, float distance) {
  if(!axis->is_setup) return;
  if(distance == 0) return;
  if(!axisSafetyCheck(axis, distance)) return;
  
  Serial.print("Moving ");
  Serial.print(axis->Name);
  Serial.print(" Axis: ");
  Serial.print(distance);
  Serial.println("mm");

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

void moveAxes(Axis *xAxis, float xDistance, Axis *yAxis, float yDistance) {
  if(!xAxis->is_setup || !yAxis->is_setup) return;
  if(xDistance == 0 || yDistance == 0) return;
  if(!axisSafetyCheck(xAxis, xDistance)) return;
  if(!axisSafetyCheck(yAxis, yDistance)) return;
  
  Serial.print("Moving X Axis: ");
  Serial.print(xDistance);
  Serial.print(" Y Axis: ");
  Serial.println(yDistance);

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

  int smallerAxisCounter = 0;

  // Move larger axis with smaller axis at ratio intervals
  for(int i = 0; i < abs(largerDistance) * pulsesPerMM; i++) {
    // Make sure we don't go negative if the limit switch is engaged
    if (!digitalRead(xAxis->LimitPin) && xDistance < 0) {
      Serial.print("Stopped moving because of X axis limit switch: ");
      break;
    } else if (!digitalRead(yAxis->LimitPin) && yDistance < 0) {
      Serial.println("Stopped moving because of Y axis limit switch");
      break;
    }

    // Always move larger axis
    digitalWrite(largerAxis->StepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(largerAxis->StepPin,LOW); 
    delayMicroseconds(500);
    
    // Move smaller axis if applicable
    smallerAxisCounter++;
    if (smallerAxisCounter == axisRatio) {
      digitalWrite(smallerAxis->StepPin,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(smallerAxis->StepPin,LOW); 
      delayMicroseconds(500);
      smallerAxisCounter = 0;
    }
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
