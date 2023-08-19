#include "WiFi.h"

void WiFi_Broadcast_Setup()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("PIA magisterka", NULL);
}