#include <BluetoothSerial.h>
#include "string.h"

BluetoothSerial SerialBT;
bool serialBTStarted = false;

void Bluetooth_Setup(String device_ID)
{
    SerialBT.begin(device_ID);
    serialBTStarted = true;
}

void Bluetooth_Write_Debug()
{
    SerialBT.print("front: ");
    SerialBT.print( getFrontDistance());
    SerialBT.print("; right: ");
    SerialBT.print(getRightDistance());
    SerialBT.print("; left: ");
    SerialBT.println(getLeftDistance());
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
    if(SerialBT.connected())
    {
        if(autoMode)
        {
            Bluetooth_Write_Debug();
        }
    }
}