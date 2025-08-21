#include "esp_err.h"
#include "light.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "server.h"
#include "esp_timer.h"

static void IRAM_ATTR udp_timer_callback(void *arg)
{
    struct server_message_req_t *server_message_req = (struct server_message_req_t *) arg;

    if (wifi_is_connected())
        server_send(server_message_req);
}

void udp_timer_start(struct server_message_req_t *server_message_req)
{
    const esp_timer_create_args_t timer_args = {
        .callback = &udp_timer_callback,
        .arg = server_message_req,
        .dispatch_method = ESP_TIMER_TASK, // ou ESP_TIMER_ISR
        .name = "udp_send"
    };

    esp_timer_handle_t timer;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer, 50000));
}

void app_main(void)
{
    struct light_t *light;
    struct server_message_req_t server_message_req;
    struct server_t *server;

    wifi_init();
    light = light_init();
    server = server_init(1234);

    gpio_reset_pin(15);
    gpio_set_direction(15, GPIO_MODE_OUTPUT);
    gpio_set_level(15, 0); // blue led on the esp pcb

    udp_timer_start(&server_message_req);

    while (1) {
        server_get(server, light);

        server_message_req.r = light->r;
        server_message_req.g = light->g;
        server_message_req.b = light->b;
        server_message_req.w = light->w;
        server_message_req.y = light->y;
        server_message_req.device_id = 3;
        server_message_req.rssi = wifi_get_rssi();

        light_ctrl(light);

        gpio_set_level(15, wifi_is_connected());
    }
    
}
