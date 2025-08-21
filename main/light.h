#ifndef __LIGHT_H
#define __LIGHT_H

#include <stdint.h>
#include "driver/ledc.h"

#define PWM_FREQ_HZ 100000
#define PWM_RES     LEDC_TIMER_8_BIT
#define PWM_TIMER   LEDC_TIMER_0
#define PWM_MODE    LEDC_LOW_SPEED_MODE

struct light_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t y;
};

struct light_t *light_init();
void light_ctrl(struct light_t *light);

#endif