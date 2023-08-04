#include "Arduino.h"
#include "../pin_config.h"

void Forward(int speed){
  analogWrite(DRIVE_A1,speed);
  analogWrite(DRIVE_A2,0);
  analogWrite(DRIVE_B1,speed);
  analogWrite(DRIVE_B2,0);
}

void Backward(int speed){
  analogWrite(DRIVE_A1,0);
  analogWrite(DRIVE_A2,speed);
  analogWrite(DRIVE_B1,0);
  analogWrite(DRIVE_B2,speed);
}

void Stop(){
  analogWrite(DRIVE_A1,0);
  analogWrite(DRIVE_A2,0);
  analogWrite(DRIVE_B1,0);
  analogWrite(DRIVE_B2,0);
}