#ifndef __WIFI_H
#define __WIFI_H

#include <string.h>
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"

#define WIFI_SSID      "BEALIGHT"
#define WIFI_PASS      "12345678"

#define ESP_IP      "192.168.1.3"
#define ESP_GW      "192.168.1.1"
#define ESP_NETMASK "255.255.255.0"


void wifi_init();
int wifi_is_connected();
int wifi_get_rssi();


#endif