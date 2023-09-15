#include "esp32_nat_router.c"

void wifi_init(const char* ssid, const char* ent_username, const char* ent_identity, const char* passwd, const char* static_ip, const char* subnet_mask, const char* gateway_addr, const char* ap_ssid, const char* ap_passwd, const char* ap_ip);
esp_err_t get_portmap_tab();

void display_netif();