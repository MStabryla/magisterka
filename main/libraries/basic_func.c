#include "Arduino.h"

char* getDistanceMess(int distanceEcho, int distanceTrig){

  digitalWrite(distanceTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(distanceTrig, HIGH);
  delayMicroseconds(15);
  digitalWrite(distanceTrig, LOW);

  long resultIn = 0;
  resultIn = pulseIn(distanceEcho, HIGH, 1000);
  resultIn /= 58;
  char* mess;
  sprintf(mess, "dist: %d cm", resultIn);

  return mess;
}

float getDistance(int distanceEcho, int distanceTrig){

  digitalWrite(distanceTrig, LOW);
  delayMicroseconds(1);
  digitalWrite(distanceTrig, HIGH);
  delayMicroseconds(15);
  digitalWrite(distanceTrig, LOW);

  long resultIn = 0;
  resultIn = pulseIn(distanceEcho, HIGH);

  float soundParameter = 58.0;
  
  return resultIn /= soundParameter;
}