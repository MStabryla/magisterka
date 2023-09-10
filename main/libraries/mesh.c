#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_mesh_internal.h"
#include "nvs_flash.h"

#define DEBUG false

#define RX_SIZE (1500)
#define TX_SIZE (1460)

void mesh_ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    Serial.print("mesh_ip event: ");
    Serial.print(event_base);
    Serial.print(" ");
    Serial.println(event_id);
}

void mesh_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    Serial.print("mesh event: ");
    switch (event_id) {
        case MESH_EVENT_STARTED: Serial.print("MESH_EVENT_STARTED ");break;
        case MESH_EVENT_STOPPED: Serial.print("MESH_EVENT_STOPPED ");break;
        case MESH_EVENT_CHANNEL_SWITCH: Serial.print("MESH_EVENT_CHANNEL_SWITCH ");break;
        case MESH_EVENT_CHILD_CONNECTED: Serial.print("MESH_EVENT_CHILD_CONNECTED ");break;
        case MESH_EVENT_CHILD_DISCONNECTED: Serial.print("MESH_EVENT_CHILD_DISCONNECTED ");break;
        case MESH_EVENT_ROUTING_TABLE_ADD: Serial.print("MESH_EVENT_ROUTING_TABLE_ADD ");break;
        case MESH_EVENT_ROUTING_TABLE_REMOVE: Serial.print("MESH_EVENT_ROUTING_TABLE_REMOVE ");break;
        case MESH_EVENT_PARENT_CONNECTED: Serial.print("MESH_EVENT_PARENT_CONNECTED ");break;
        case MESH_EVENT_PARENT_DISCONNECTED: Serial.print("MESH_EVENT_PARENT_DISCONNECTED ");break;
        case MESH_EVENT_NO_PARENT_FOUND: Serial.print("MESH_EVENT_NO_PARENT_FOUND ");break;
        case MESH_EVENT_LAYER_CHANGE: Serial.print("MESH_EVENT_LAYER_CHANGE ");break;
        case MESH_EVENT_TODS_STATE: Serial.print("MESH_EVENT_TODS_STATE ");break;
        case MESH_EVENT_VOTE_STARTED: Serial.print("MESH_EVENT_VOTE_STARTED ");break;
        case MESH_EVENT_VOTE_STOPPED: Serial.print("MESH_EVENT_VOTE_STOPPED ");break;
        case MESH_EVENT_ROOT_ADDRESS: Serial.print("MESH_EVENT_ROOT_ADDRESS ");break;
        case MESH_EVENT_ROOT_SWITCH_REQ: Serial.print("MESH_EVENT_ROOT_SWITCH_REQ ");break;
        case MESH_EVENT_ROOT_SWITCH_ACK: Serial.print("MESH_EVENT_ROOT_SWITCH_ACK ");break;
        case MESH_EVENT_ROOT_ASKED_YIELD: Serial.print("MESH_EVENT_ROOT_ASKED_YIELD ");break;
        case MESH_EVENT_ROOT_FIXED: Serial.print("MESH_EVENT_ROOT_FIXED ");break;
        case MESH_EVENT_SCAN_DONE: Serial.print("MESH_EVENT_SCAN_DONE ");break;
        case MESH_EVENT_NETWORK_STATE: Serial.print("MESH_EVENT_NETWORK_STATE ");break;
        case MESH_EVENT_STOP_RECONNECTION: Serial.print("MESH_EVENT_STOP_RECONNECTION ");break;
        case MESH_EVENT_FIND_NETWORK: Serial.print("MESH_EVENT_FIND_NETWORK ");break;
        case MESH_EVENT_ROUTER_SWITCH: Serial.print("MESH_EVENT_ROUTER_SWITCH ");break;
        case MESH_EVENT_PS_PARENT_DUTY: Serial.print("MESH_EVENT_PS_PARENT_DUTY ");break;
        case MESH_EVENT_PS_CHILD_DUTY: Serial.print("MESH_EVENT_PS_CHILD_DUTY ");break;
        case MESH_EVENT_PS_DEVICE_DUTY: Serial.print("MESH_EVENT_PS_DEVICE_DUTY ");break;
        case MESH_EVENT_MAX: Serial.print("MESH_EVENT_MAX ");break;
    default:
        break;
    }
    Serial.println(event_id);
}

static wifi_init_config_t config;
void Mesh_Module_Init()
{
    //Inicjalizacja warstwy LwIP
    #if DEBUG == true
        Serial.println("DEBUG - lwIP");
    #endif
    ESP_ERROR_CHECK(esp_netif_init());

    /* Konfiguracja domyślnych eventów */
    #if DEBUG == true
        Serial.println("DEBUG - event loop");
    #endif
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /*  Wi-Fi initialization */
    #if DEBUG == true
        Serial.println("DEBUG - wifi config");
    #endif
    config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));
    /*  Obsługa zdarzeń podczas połączenia WiFI */
    #if DEBUG == true
        Serial.println("DEBUG - wifi handler");
    #endif
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &mesh_ip_event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    #if DEBUG == true
        Serial.println("DEBUG - wifi start");
    #endif
    ESP_ERROR_CHECK(esp_wifi_start());
}

//TESTY W DOMU
static const int CONFIG_MESH_MAX_LAYER = 3;
static const uint8_t MESH_ID[6] = { 0x77, 0x77, 0x77, 0x77, 0x77, 0x77};
static const uint8_t CONFIG_MESH_CHANNEL = 0x04;
static const String CONFIG_MESH_ROUTER_SSID = "MAXX_LAN";
static const String CONFIG_MESH_ROUTER_PASSWD = "debina23";
static const uint8_t CONFIG_MESH_AP_CONNECTIONS = 0x03;
static const uint8_t CONFIG_NONMESH_AP_CONNECTIONS = 0x07;
static const String CONFIG_MESH_AP_PASSWD = "debina23";
static wifi_auth_mode_t CONFIG_MESH_AP_AUTHMODE = WIFI_AUTH_WPA2_PSK;

void Mesh_Init()
{
    ESP_ERROR_CHECK(esp_mesh_init());
    ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &mesh_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mesh_set_topology(MESH_TOPO_TREE));
    /*  set mesh max layer according to the topology */
    ESP_ERROR_CHECK(esp_mesh_set_max_layer(CONFIG_MESH_MAX_LAYER));
    ESP_ERROR_CHECK(esp_mesh_set_vote_percentage(1));
    ESP_ERROR_CHECK(esp_mesh_set_ap_assoc_expire(10));
}

void Mesh_Network_Config()
{
    /* Enable the Mesh IE encryption by default */
    mesh_cfg_t mesh_config = MESH_INIT_CONFIG_DEFAULT();
    //mesh_config.crypto_funcs = &g_wifi_default_mesh_crypto_funcs;
    mesh_config.allow_channel_switch = true;
    /* mesh ID */
    memcpy((uint8_t *) &mesh_config.mesh_id, MESH_ID, 6);
    /* channel (must match the router's channel) */
    mesh_config.channel = CONFIG_MESH_CHANNEL;
    /* router */
    mesh_config.router.allow_router_switch = true;
    mesh_config.router.ssid_len = strlen(CONFIG_MESH_ROUTER_SSID.c_str());
    memcpy((uint8_t *) &mesh_config.router.ssid, 
    (uint8_t *)CONFIG_MESH_ROUTER_SSID.c_str(), mesh_config.router.ssid_len);
    memcpy((uint8_t *) &mesh_config.router.password, 
    (uint8_t *)CONFIG_MESH_ROUTER_PASSWD.c_str(),
        CONFIG_MESH_ROUTER_PASSWD.length());
    /* mesh softAP */
    ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(CONFIG_MESH_AP_AUTHMODE));
    memcpy((uint8_t *) &mesh_config.mesh_ap.password,(uint8_t *) CONFIG_MESH_AP_PASSWD.c_str(), CONFIG_MESH_AP_PASSWD.length());
    mesh_config.mesh_ap.max_connection = CONFIG_MESH_AP_CONNECTIONS;
    mesh_config.mesh_ap.nonmesh_max_connection = CONFIG_NONMESH_AP_CONNECTIONS;
    #if DEBUG == true
        Serial.println("DEBUG mesh_config:");
        Serial.print("    mesh_ap : "); Serial.print((char*)mesh_config.mesh_ap.password); Serial.print(", "); Serial.print(mesh_config.mesh_ap.max_connection); Serial.print(", "); Serial.println(mesh_config.mesh_ap.nonmesh_max_connection);
        Serial.print("    router : "); Serial.print(mesh_config.router.allow_router_switch); Serial.print(", "); Serial.print((char*)mesh_config.router.ssid); Serial.print(", "); Serial.print((char*)mesh_config.router.bssid); Serial.print(", "); Serial.print((char*)mesh_config.router.password); Serial.print(", "); Serial.println(mesh_config.router.ssid_len);
        Serial.print("    channel : "); Serial.println(mesh_config.channel);
        Serial.print("    allow_channel_switch : "); Serial.println(mesh_config.allow_channel_switch);
        Serial.print("    mesh_id : "); Serial.print((char*)mesh_config.mesh_id.addr); Serial.print(", "); Serial.print(mesh_config.mesh_id.mip.ip4.addr); Serial.print(":"); Serial.println(mesh_config.mesh_id.mip.port);
    #endif   
    ESP_ERROR_CHECK(esp_mesh_set_config(&mesh_config));
    Serial.println("Mesh network configured.");
}

bool mesh_stated = false;

void Mesh_Start()
{
    //esp_mesh_set_self_organized(true, true);
    ESP_ERROR_CHECK(esp_mesh_start());
    esp_mesh_set_self_organized(true, true);
    Serial.println("Mesh self organization started.");
    mesh_stated = true;
}