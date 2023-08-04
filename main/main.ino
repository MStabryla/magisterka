#include "Arduino.h"
#include "libraries/basic_func.h"
#include "libraries/movement.h"

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

void debug_loop_distance(){

  float distanceF = getDistance(ECHO_1,TRIG_1);
  delay(5);
  float distanceR = getDistance(ECHO_2,TRIG_2);
  delay(5);
  float distanceL = getDistance(ECHO_3,TRIG_3);
  
  Serial.print("front: ");
  Serial.print(distanceF);

  Serial.print("; right: ");
  Serial.print(distanceR);

  Serial.print("; left: ");
  Serial.println(distanceL);

  addMeasureToDistanceArray('f',distanceF);
  addMeasureToDistanceArray('r',distanceR);
  addMeasureToDistanceArray('l',distanceL);
  
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

void debug_loop_movement(){
    //Serial.println("forward");
    Forward(100);
    delay(5000);
    //Serial.println("backward");
    Backward(100);
    delay(5000);
    //Serial.println("stop");
    Stop();
}


void debug_loop(){
    debug_loop_distance();
    //debug_loop_movement();
}




void loop() {

  // DEBUG
  debug_loop();

  delay(1000);
  
}

