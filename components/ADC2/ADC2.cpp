#include <stdio.h>
#include "ADC2.h"

ADC2::ADC2(/* args */)
{
}

ADC2::~ADC2()
{
    //TODO
}

esp_err_t ADC2::init(adc_bits_width_t _adc_bits_width, adc2_channel_t adc_channel,adc_atten_t adc_atten)
{
    channel = adc_channel;
    bits_width = _adc_bits_width;
    return adc2_config_channel_atten(adc_channel, adc_atten);
}

uint32_t ADC2::read()
{
    int value;
    ESP_ERROR_CHECK(adc2_get_raw(channel, bits_width, &value));
    return value;
}

esp_err_t ADC2::changeChannel( adc2_channel_t adc_channel,adc_atten_t adc_atten)
{
    channel = adc_channel;
    return adc2_config_channel_atten(adc_channel, adc_atten);
}

void ADC2::changeBitsWidth(adc_bits_width_t adc_bits_width)
{
    bits_width = adc_bits_width;
}



