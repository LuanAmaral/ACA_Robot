/* 
 *    encoder.h
 *   
 *    Creator: Luan Amaral
 *    21/04/2021
 */ 

#ifndef encoder_H_
#define encoder_H_

#include "driver/pcnt.h"

#define PI 3.14159265

class encoder
{
private:
    uint8_t pin_a;
    uint8_t pin_b;
    pcnt_unit_t unit;

public:
    uint16_t pulse_per_rev;
    pcnt_config_t enc;

    encoder(uint8_t _pin_a, uint8_t _pin_b, uint16_t _pulse_rev, pcnt_channel_t channel, pcnt_unit_t unit );
    ~encoder();
    esp_err_t get_dist_angular_rad(float *rad);
};


#endif


