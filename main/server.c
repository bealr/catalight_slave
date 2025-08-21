#include "server.h"
#include "lwip/sockets.h"
#include <string.h>
#include <stdio.h>

void server_send(struct server_message_req_t *server_message_req) {

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(UDP_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(UDP_HOST);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    sendto(sock, server_message_req, sizeof(struct server_message_req_t), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    
    close(sock);
}

struct server_t *server_init(int port) {

    struct server_t *server = (struct server_t *) malloc(sizeof(struct server_t));
    int i;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    server->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    bind(server->sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    // Mettre socket en non bloquant
    int flags = fcntl(server->sock, F_GETFL, 0);
    fcntl(server->sock, F_SETFL, flags | O_NONBLOCK);

    return server;
}

int server_get(struct server_t *server, struct light_t *light) {

    char rx_buffer[128];
    struct sockaddr_in source_addr;
    socklen_t socklen = sizeof(source_addr);
    struct server_message_req_t msg_req;

    int len = recvfrom(server->sock, rx_buffer, sizeof(rx_buffer) - 1, 0,
                        (struct sockaddr *)&source_addr, &socklen);

    if (len != sizeof(struct server_message_req_t))
        return 0;

    memcpy(&msg_req, rx_buffer, sizeof(struct server_message_req_t));
    printf("UDP from: %d\n", msg_req.device_id);

    printf("r : %d\n", msg_req.r);

    if (msg_req.device_id == 0) {

        light->r = msg_req.r;
        light->g = msg_req.g;
        light->b = msg_req.b;
        light->w = msg_req.w;
        light->y = msg_req.y;
    }

    return 0;
}
