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

void addMeasureToDistanceArray(char measureIndex, int measure)
{
  for(int i=0;i<DISTANCE_ARRAY_COUNT-1;i++)
  {
    switch(measureIndex)
    {
      case 'f': _distanceArrayF[i+1] = _distanceArrayF[i]; break;
      case 'r': _distanceArrayR[i+1] = _distanceArrayR[i]; break;
      case 'l': _distanceArrayL[i+1] = _distanceArrayL[i]; break;
    }
  }
  switch(measureIndex)
  {
    case 'f': _distanceArrayF[0] = measure; break;
    case 'r': _distanceArrayR[0] = measure; break;
    case 'l': _distanceArrayL[0] = measure; break;
  }
}