#ifndef __SERVER_H
#define __SERVER_H

#include <stdint.h>
#include "light.h"

struct server_message_req_t {
    uint8_t device_id;
    int8_t rssi;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t y;
    uint8_t voltage;
};

struct server_t {
    int sock;
};

#define UDP_HOST    "192.168.1.1"
#define UDP_PORT    1234

void server_send(struct server_message_req_t *server_message_req);
struct server_t *server_init(int port);
int server_get(struct server_t *server, struct light_t *light);

#endif