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
    uint16_t pulse_per_rev;
    pcnt_config_t enc;

public:
    encoder();
    ~encoder();
    esp_err_t init(uint8_t _pin_a, uint8_t _pin_b, uint16_t _pulse_rev, pcnt_channel_t channel, pcnt_unit_t unit);
    esp_err_t getDistAngularRad(float *rad);
};
#endif


