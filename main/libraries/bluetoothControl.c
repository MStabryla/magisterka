#include <BluetoothSerial.h>
#include "string.h"

#define BT_ID "MAG_2"

BluetoothSerial SerialBT;

void Bluetooth_Setup()
{
    SerialBT.begin(BT_ID);
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
        else if(strcmp(bluetoothCommand.c_str(), "stop"))
        {
            autoMode = false;
        }
    }
}