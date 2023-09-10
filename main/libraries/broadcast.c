#include "WiFi.h"
#include "string.h"
#include "bridge_wifi.h"
#include "bridge/esp_bridge.h"

#define TESTY_SIECI "MAXX_LAN"
#define TESTY_SIECI_PASSWD  "debina23"

#define WIFI_DEBUG true

void WiFi_Broadcast_Setup()
{
    WiFi.mode(WIFI_AP_STA);
    //WiFi.disconnect();
}

String selectedNetworkSSID;
int selectedNetworkRSSI = INT_MIN; 

void NewConnectionEventHandler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
    Serial.print("EVENT NEW_CONNECTION: ");
    Serial.println((char *)&wifi_info.wifi_ap_staconnected.mac);
}

bool WiFi_Scan_Connect(String device_ID)
{
    if(WiFi.isConnected())
        return true;
    int networksCount = WiFi.scanNetworks();
    if(networksCount > 0)
    {
        for(int i = 0;i < networksCount; i++)
        {
            String ssid = WiFi.SSID(i);
            int signalPower = WiFi.RSSI(i);
            #if DEBUG == true
                Serial.print("Found network ");
                Serial.print(ssid);
                Serial.print(" ");
                Serial.print(signalPower);
                Serial.println(" dBm");
            #endif
            if(WiFi.encryptionType(i) == WIFI_AUTH_OPEN || (ssid == TESTY_SIECI))
            {
                if(signalPower > selectedNetworkRSSI)
                {
                    selectedNetworkSSID = ssid;
                    //memcpy((String *) &selectedNetworkSSID, ssid.c_str(), ssid.length());
                    selectedNetworkRSSI = signalPower;
                }
            }
        }
        WiFi.scanDelete();

        //Łączenie
        #if DEBUG == true
            Serial.print("Connecting to network ");
            Serial.print(selectedNetworkSSID);
            Serial.print(" ");
            Serial.print(selectedNetworkRSSI);
            Serial.println(" dBm");
        #endif

        if(selectedNetworkSSID == TESTY_SIECI) 
            WiFi.begin(selectedNetworkSSID, TESTY_SIECI_PASSWD);
        else
            WiFi.begin(selectedNetworkSSID);

        int connectionCounter = 100;
        while(WiFi.status() != WL_CONNECTED && connectionCounter > 0)
        {
            delay(200);
            connectionCounter = connectionCounter - 1;
        }
        if(WiFi.status() != WL_CONNECTED)
            return false;
        Serial.print("Connected to network ");
        // Serial.println(selectedNetworkSSID);
        #if DEBUG == true
            Serial.println("Starting AP");
        #endif
        //WiFi.softAP(device_ID.c_str(), NULL);
        //WiFi.onEvent(NewConnectionEventHandler,ARDUINO_EVENT_WIFI_AP_STACONNECTED);
        //esp_bridge_create_all_netif();

        // ESP_ERROR_CHECK(esp_netif_init());
        // ESP_ERROR_CHECK(esp_event_loop_create_default());

        // esp_bridge_create_softap_netif(NULL, NULL, true, true);
        // esp_bridge_wifi_set(WIFI_MODE_APSTA, TESTY_SIECI, TESTY_SIECI_PASSWD, NULL);
        
        // Serial.println("AP started");
        return true;
    }
}

bool Wifi_Scan_Connect_2()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    //esp_bridge_wifi_init();
    //ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    //esp_bridge_create_softap_netif(NULL, NULL, true, true);
    esp_bridge_create_station_netif(NULL, NULL, false, false, TESTY_SIECI, TESTY_SIECI_PASSWD);
    ESP_ERROR_CHECK(esp_bridge_wifi_set(WIFI_MODE_STA, TESTY_SIECI, TESTY_SIECI_PASSWD, NULL));
    //esp_bridge_wifi_set(WIFI_MODE_AP, "TEST_MAXX_LAN", "", NULL);
    //ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
    Serial.println("AP started");
    return true;
}