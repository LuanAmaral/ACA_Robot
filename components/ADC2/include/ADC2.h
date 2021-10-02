/*  
    Classe para leitura ADC ESP32
    Luan R Amaral
*/

#ifndef __ADC2_H__
#define __ADC2_H__

#include <stdio.h>
#include "esp_attr.h"
#include "driver/gpio.h"
#include "driver/adc.h"

class ADC2
{
private:
    adc2_channel_t channel;
    adc_bits_width_t bits_width;
public:
    ADC2();
    ~ADC2();
    esp_err_t init(adc_bits_width_t _adc_bits_width, adc2_channel_t adc_channel,adc_atten_t adc_atten);
    uint32_t read();
    esp_err_t changeChannel(adc2_channel_t adc_channel, adc_atten_t adc_atten);
    void changeBitsWidth(adc_bits_width_t adc_bits_width);
};

#endif