/*  
    Classe para leitura ADC ESP32
    Luan R Amaral
*/

#ifndef __ADC1_H__
#define __ADC1_H__

#include <stdio.h>
#include <stdlib.h>
#include "esp_attr.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <esp_log.h>

#define DEFAULT_VREF    1100
#define TAG "ADC1"

class ADC1
{
private:
    adc1_channel_t channel;
    adc_atten_t atten;
    adc_bits_width_t bits_width;
    esp_adc_cal_characteristics_t adc_calib;
public:
    ADC1();
    ~ADC1();
    esp_err_t init(adc_bits_width_t adc_bits_width, adc1_channel_t adc_channel, adc_atten_t adc_atten);
    int read(int samples);
    esp_err_t changeChannel(adc1_channel_t adc_channel, adc_atten_t adc_atten);
    uint32_t readVolts(int samples);
};















#endif