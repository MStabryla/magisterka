#include "Arduino.h"
#include "lib/basic_func.h"
#include "pin_config.h"



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("test");
  delay(5000);
}
