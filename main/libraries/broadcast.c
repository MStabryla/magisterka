#define CONFIG_LWIP_DEBUG 1

#include "WiFi.h"
#include "string.h"
#include "esp_vfs_dev.h"
#include "lwip/lwip_napt.h"

// #define TESTY_SIECI "MAXX_LAN"
// #define TESTY_SIECI_PASSWD  "debina23"

#define TESTY_SIECI "Atlantis"
#define TESTY_SIECI_PASSWD  "zaq1@WSX"

#define DEBUG true

void WiFi_Broadcast_Setup()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
}

String selectedNetworkSSID;
int selectedNetworkRSSI = INT_MIN; 

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
        //Łączenie do istniejącej sieci WiFi
        if(selectedNetworkSSID == NULL)
            return false;
        if(selectedNetworkSSID == TESTY_SIECI) 
            WiFi.begin(selectedNetworkSSID, TESTY_SIECI_PASSWD);
        else
            WiFi.begin(selectedNetworkSSID);
        //Oczekiwanie na połączenie z siecią
        int connectionCounter = 100;
        while(WiFi.status() != WL_CONNECTED && connectionCounter > 0)
        {
            delay(200);
            connectionCounter = connectionCounter - 1;
        }
        if(WiFi.status() != WL_CONNECTED)
            return false;
        #if DEBUG == true
            Serial.print("Connected to network ");
            Serial.println(selectedNetworkSSID);
            Serial.println("Starting AP");
        #endif
        //Rozprowadzenie własnego sygnału AP
        WiFi.softAP(device_ID.c_str(), NULL,4);
        delay(1000);
        //Ustawienie translacji NAT
        ip_napt_enable(WiFi.softAPIP(),1);
        // Ustawienie domyślnego serwera DNS
        dhcps_offer_t dhcps_dns_value = OFFER_DNS;
        dhcps_set_option_info(6, &dhcps_dns_value, sizeof(dhcps_dns_value));
        ip_addr_t dnsserver; dnsserver.u_addr.ip4.addr = ipaddr_addr("8.8.8.8"); dnsserver.type = IPADDR_TYPE_V4;
        dhcps_dns_setserver(&dnsserver);

        digitalWrite(BLINK_GPIO,HIGH);
        Serial.println("AP started");
        return true;
    }
}


// bool Wifi_Scan_Connect_2(String device_ID)
// {
//     initialize_nvs();
//     get_portmap_tab();
//     Serial.print("Starting AP ");
//     Serial.println(device_ID);
//     wifi_init(TESTY_SIECI,"","", TESTY_SIECI_PASSWD,"","","", device_ID.c_str(), "", DEFAULT_AP_IP);

//     delay(3000);
//     ip_napt_enable(my_ap_ip, 1);

//     digitalWrite(BLINK_GPIO,HIGH);
//     Serial.println("AP started");
//     return true;
// }