/*
 *     encoder.cpp
 * 
 *     Creator: Luan Amaral
 * 
 */

#include "encoder.h"
#include <stdio.h>

encoder::encoder(uint8_t _pin_a, uint8_t _pin_b, uint16_t _pulse_per_rev, pcnt_channel_t channel, pcnt_unit_t _unit )
{
    pin_a = _pin_a;
    pin_b = _pin_b;
    pulse_per_rev = _pulse_per_rev;
    unit = _unit;

    enc.pulse_gpio_num = pin_a;
    enc.ctrl_gpio_num = pin_b;
    enc.channel = channel;
    enc.unit = unit;
    enc.pos_mode = PCNT_COUNT_DEC;
    enc.neg_mode = PCNT_COUNT_INC;
    enc.lctrl_mode = PCNT_MODE_REVERSE;
    enc.hctrl_mode = PCNT_MODE_KEEP;
    enc.counter_h_lim = 0xFFF;
    enc.counter_l_lim = -0xFFF;

    pcnt_unit_config(&enc);
}

encoder::~encoder()
{
}

esp_err_t encoder::get_dist_angular_rad(float *rad)
{
    int16_t pulses;
    if(pcnt_get_counter_value(unit, &pulses) == ESP_OK)
    {
        *rad = ((float)pulses / pulse_per_rev) * PI * 2;
        pcnt_counter_clear(unit);
        return ESP_OK;
    }
    return ESP_FAIL;
}