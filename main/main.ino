// #define CONFIG_LWIP_IPV4 1
// #define CONFIG_LWIP_IP_FORWARD 1
// #define CONFIG_LWIP_IPV4_NAPT 1
// #define CONFIG_LWIP_L2_TO_L3_COPY 1
#define BLINK_GPIO 2

#include "Arduino.h"
#include "libraries/broadcast.h"
#include "libraries/autonomousSystem.h"
#include "libraries/bluetoothControl.h"
#include "libraries/mesh.h"
#include "EEPROM.h"
#include "ESPping.h"

String device_ID = "TEST";

#define CODE_TYPE 1

void setup() {

  EEPROM.begin(8);

  // EEPROM.writeString(1,"MAG_3");
  // EEPROM.commit();

  device_ID = EEPROM.readString(1);
  
  Serial.begin(115200);
  // FRONT: 32 - TRIG; 35 - ECHO
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);

  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);

  pinMode(TRIG_3, OUTPUT);
  pinMode(ECHO_3, INPUT);

  pinMode(BLINK_GPIO,OUTPUT);

  //diode
  pinMode(DIODE_AUTO_MODE, OUTPUT);

  Serial.print("ID: ");
  Serial.println(device_ID);

  #if CODE_TYPE == 2
    WiFi_Broadcast_Setup();
    Bluetooth_Setup(device_ID);
  #endif
  
}

int TESTCOUNTER = 0;

void debug_loop_distance(){

  //odległośc z przodu
  float distanceF = getDistance(ECHO_1,TRIG_1);
  delay(5);
  //odległośc z prawej
  float distanceR = getDistance(ECHO_2,TRIG_2);
  delay(5);
  //odległość z lewej
  float distanceL = getDistance(ECHO_3,TRIG_3);
  
  Serial.print("front: ");
  Serial.print(distanceF);

  Serial.print("; right: ");
  Serial.print(distanceR);

  Serial.print("; left: ");
  Serial.println(distanceL);

  addMeasureToDistanceArray('f',(int)distanceF);
  addMeasureToDistanceArray('r',(int)distanceR);
  addMeasureToDistanceArray('l',(int)distanceL);
  
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
    Serial.println("forward");
    Forward(255);
    delay(5000);
    Serial.println("backward");
    Backward(255);
    delay(5000);
    Serial.println("stop");
    Stop();
    delay(1000);
}

void debug_loop_movement_turn(){
    Serial.println("left");
    Left(255);
    delay(5000);
    Serial.println("right");
    Right(255);
    delay(5000);
    Serial.println("stop");
    Stop();
}

void autonomousMovement(){
  if(autonomousMove())
  {
    delay(MILI_TO_TURN);
  }
}

void debug_loop(){
  debug_loop_distance();
  debug_loop_movement();
}

void Mesh()
{
  Mesh_Prepare_Network();
  Mesh_Init();
  Mesh_Network_Config();
  Mesh_Start();
}
/*
  0 - inicjalizacja mesh
  1 - poruszanie się autonomiczne
  2 - wyłączanie autonomicznego poruszania się i mesh
  3 - przełączenie się na tryb AP
*/
int phase = 0, actPhase = 0;

void loop() {
  digitalWrite(DIODE_AUTO_MODE,autoMode);
  #if CODE_TYPE == 0
    if(phase == 0)
    {
      if(!autoMode)
        Mesh();
      else
        phase = 1;  
    }
    else if( phase == 1)
    {
      autonomousMovement();
      if(!autoMode)
        phase = 2;
    }
    else if( phase == 2)
    {
      if(mesh_stated)
      {
        Mesh_Stop();
        WiFi_Broadcast_Setup();
      }
      while(!WiFi_Scan_Connect(device_ID.c_str()))
      {
        Serial.printf("WiFi connection not established. Restarting scan ...");
        delay(1000);
      }
    }
  #endif
  #if CODE_TYPE == 1
    if(autoMode)
    {
      autonomousMovement();
    }
  #endif
  #if CODE_TYPE == 2
    while(!WiFi_Scan_Connect(device_ID.c_str()))
    {
      Serial.printf("WiFi connection not established. Restarting scan ...");
      delay(1000);
    }
  #endif
  #if CODE_TYPE == 3
    if(!mesh_stated)
    {
      Mesh();
    }
  #endif
}
