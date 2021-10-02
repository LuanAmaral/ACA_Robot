#include <stdio.h>
#include "ADC1.h"

ADC1::ADC1(/* args */)
{
}

ADC1::~ADC1()
{
    //TODO
}

esp_err_t ADC1::init(adc_bits_width_t adc_bits_width, adc1_channel_t adc_channel,adc_atten_t adc_atten)
{
    channel = adc_channel;
    atten = adc_atten;
    bits_width = adc_bits_width;
    adc1_config_width(adc_bits_width);
    esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, atten, bits_width, DEFAULT_VREF, &adc_calib);
   
    if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
	{
		ESP_LOGI(TAG, "Vref eFuse encontrado: %umV", adc_calib.vref);
	}
	else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP)
	{
		ESP_LOGI(TAG, "Two Point eFuse encontrado");
	}
	else
	{
		ESP_LOGW(TAG, "Nada encontrado, utilizando Vref padrao: %umV", adc_calib.vref);
	}
   
    return adc1_config_channel_atten(channel, adc_atten);
}

int ADC1::read(int samples)
{
    int reading = 0;
    for (int i = 0; i < samples; i++)
    {
        reading += adc1_get_raw(channel);
    }
    return reading/samples;
}

esp_err_t ADC1::changeChannel(adc1_channel_t adc_channel,adc_atten_t adc_atten)
{
    channel = adc_channel;
    return adc1_config_channel_atten(channel, adc_atten);
}

uint32_t ADC1::readVolts(int samples)
{
    uint32_t reading = read(samples);
    uint32_t voltage = esp_adc_cal_raw_to_voltage(reading, &adc_calib);
    return voltage;
}







