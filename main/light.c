#include "light.h"

struct light_t *light_init() {

    struct light_t *light = (struct light_t *) malloc(sizeof(struct light_t));

    ledc_timer_config_t ledc_timer = {
    .speed_mode       = PWM_MODE,
    .timer_num        = PWM_TIMER,
    .duty_resolution  = PWM_RES,
    .freq_hz          = PWM_FREQ_HZ,
    .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    for (int ch = 0; ch < 5; ch++) {
        ledc_channel_config_t ledc_channel = {
            .speed_mode     = PWM_MODE,
            .channel        = ch,
            .timer_sel      = PWM_TIMER,
            .intr_type      = LEDC_INTR_DISABLE,
            .gpio_num       = 1 + ch,   // IO1 to IO5
            .duty           = 0,
            .hpoint         = 0
        };
        ledc_channel_config(&ledc_channel);
    }

    light->r = 0;
    light->g = 0;
    light->b = 0;
    light->w = 0;
    light->y = 0;

    return light;
}

void light_ctrl(struct light_t *light) {
    
    uint8_t duty = 0;

    for (int ch = 0; ch < 5; ch++) {
        
        switch (ch) {
            case 0:
                duty = light->g /2;
                break;
            case 1:
                duty = light->r /2;
                break;
            case 2:
                duty = light->b /2;
                break;
            case 3:
                duty = light->w /2;
                break;
            case 4:
                duty = light->y /2;
                break;
        }
        
        if (duty > 128) duty = 128;

        ledc_set_duty(PWM_MODE, ch, duty);
        ledc_update_duty(PWM_MODE, ch);
        //ledc_set_duty_and_update(PWM_MODE, ch, duty, 0);
    }
}
