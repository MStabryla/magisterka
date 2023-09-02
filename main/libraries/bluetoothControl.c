#include <BluetoothSerial.h>
#include "string.h"

BluetoothSerial SerialBT;

void Bluetooth_Setup(String device_ID)
{
    SerialBT.begin(device_ID);
}

void Bluetooth_Loop()
{
    String bluetoothCommand = "";
    if(SerialBT.available()) {
        bluetoothCommand = SerialBT.readString();
        if(strcmp(bluetoothCommand.c_str(), "start") == 0)
        {
            autoMode = true;
        }
        else if(strcmp(bluetoothCommand.c_str(), "stop") == 0)
        {
            autoMode = false;
        }
    }
}