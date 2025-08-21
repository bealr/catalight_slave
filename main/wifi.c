#include "wifi.h"

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // si déconnecté → on tente de reconnecter
        esp_wifi_connect();
    }
}

void wifi_init() {
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();

    // Désactiver DHCP et mettre IP fixe
    esp_netif_dhcpc_stop(sta_netif);

    esp_netif_ip_info_t ip_info;
    inet_pton(AF_INET, ESP_IP, &ip_info.ip);
    inet_pton(AF_INET, ESP_GW, &ip_info.gw);
    inet_pton(AF_INET, ESP_NETMASK, &ip_info.netmask);
    esp_netif_set_ip_info(sta_netif, &ip_info);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char*)wifi_config.sta.password, WIFI_PASS);

    esp_event_handler_instance_register(WIFI_EVENT,
                                    WIFI_EVENT_STA_DISCONNECTED,
                                    &wifi_event_handler,
                                    NULL,
                                    NULL);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();
}

int wifi_is_connected() {
    wifi_ap_record_t ap_info;
    return (esp_wifi_sta_get_ap_info(&ap_info) == 0);
}

int wifi_get_rssi() {
    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) == 0) {
        return ap_info.rssi; // RSSI in dBm
    }
    return 0;
}