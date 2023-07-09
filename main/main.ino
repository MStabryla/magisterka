#include "Arduino.h"
#include "libraries/basic_func.h"
#include "pin_config.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // FRONT: 32 - TRIG; 35 - ECHO
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);

  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);

  pinMode(TRIG_3, OUTPUT);
  pinMode(ECHO_3, INPUT);
}

int TESTCOUNTER = 0;
void loop() {
  // odczyt odległości w cm:
  float distanceF = getDistance(ECHO_1,TRIG_1);
  delay(5);
  float distanceR = getDistance(ECHO_2,TRIG_2);
  delay(5);
  float distanceL = getDistance(ECHO_3,TRIG_3);
  //float distanceL = getDistance(ECHO_3,TRIG_3);
  Serial.print("front: ");
  Serial.print(distanceF);

  Serial.print("; right: ");
  Serial.print(distanceR);

  Serial.print("; left: ");
  Serial.println(distanceL);

  addMeasureToDistanceArray('f',distanceF);
  addMeasureToDistanceArray('r',distanceR);
  addMeasureToDistanceArray('l',distanceL);

  delay(1000);

  if(++TESTCOUNTER > 10)
  {
    TESTCOUNTER = 0;
    Serial.print("TEST front:  ");
    for(int i=0;i<DISTANCE_ARRAY_COUNT;i++)
    {
      Serial.print(_distanceArrayF[i]);
      Serial.print(" ");
    }
    Serial.println(" ");

    Serial.print("TEST right:  ");
    for(int i=0;i<DISTANCE_ARRAY_COUNT;i++)
    {
      Serial.print(_distanceArrayR[i]);
      Serial.print(" ");
    }
    Serial.println(" ");

    Serial.print("TEST left:  ");
    for(int i=0;i<DISTANCE_ARRAY_COUNT;i++)
    {
      Serial.print(_distanceArrayL[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
}
