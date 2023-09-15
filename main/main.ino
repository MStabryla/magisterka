#define CONFIG_LWIP_IPV4 1
#define CONFIG_LWIP_IP_FORWARD 1
#define CONFIG_LWIP_IPV4_NAPT 1
#define CONFIG_LWIP_L2_TO_L3_COPY 1

#include "Arduino.h"
#include "libraries/broadcast.h"
#include "libraries/autonomousSystem.h"
#include "libraries/bluetoothControl.h"
#include "libraries/mesh.h"
#include "EEPROM.h"
#include "ESPping.h"

String device_ID = "TEST";

void setup() {

  EEPROM.begin(8);

  EEPROM.writeString(1,"MAG_3");
  EEPROM.commit();

  device_ID = EEPROM.readString(1);
  
  Serial.begin(115200);
  // FRONT: 32 - TRIG; 35 - ECHO
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);

  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);

  pinMode(TRIG_3, OUTPUT);
  pinMode(ECHO_3, INPUT);

  //diode
  pinMode(DIODE_AUTO_MODE, OUTPUT);

  Serial.print("ID: ");
  Serial.println(device_ID);
  delay(1000);
  // Mesh_Module_Init();
  // Mesh_Init();
  // Mesh_Network_Config();
  // Mesh_Start();
  //WiFi_Broadcast_Setup();
  // Bluetooth_Setup(device_ID);
  Wifi_Scan_Connect_2(device_ID);
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
  // Bluetooth_Loop();
  // autonomousMovement();
  // if(!autoMode)
  // {
  //   Stop();
  // }
}

void loop() {
  // DEBUG
  //debug_loop();
  // Bluetooth_Loop();
  // autonomousMovement();
  digitalWrite(DIODE_AUTO_MODE,autoMode);
  // while(!WiFi_Scan_Connect(device_ID.c_str()))
  // {
  //   Serial.printf("WiFi connection not established. Restarting scan ...");
  //   delay(1000);
  // }
  
  if(mesh_stated)
  {
    //jak działa mesh
  }

  // bool success = Ping.ping("www.google.com", 4);
  // if(!success){
  //   Serial.println("Ping failed");
  // }
  // else
  //   Serial.println("Ping succesful.");
  // delay(5000);
}
