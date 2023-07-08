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
}

void loop() {
  // put your main code here, to run repeatedly:
  float distanceF = getDistance(ECHO_1,TRIG_1);
  delay(5);
  float distanceR = getDistance(ECHO_2,TRIG_2);
  //float distanceL = getDistance(ECHO_3,TRIG_3);
  Serial.print("front: ");
  Serial.print(distanceF);

  Serial.print("; right: ");
  Serial.println(distanceR);

  // Serial.print("; left: ");
  // Serial.println(distanceL);

  delay(1000);
}
