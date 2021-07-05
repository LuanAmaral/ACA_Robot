/*
 *  motor_dc.h
 *
 *  Created on: 16 jan 2021
 *      Author: Luan Amaral
 */
#ifndef motor_dc_H_
#define motor_dc_H_
#endif

#include <stdio.h>
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"



class motorDC
{
    private:
        int pin_foward;
        int pin_backward;
        int max_velocity;
        mcpwm_io_signals_t io_signals_foward;   
        mcpwm_io_signals_t io_signals_backward;
        mcpwm_generator_t gen_foward;
        mcpwm_generator_t gen_backward; 

        mcpwm_unit_t mcpwm_num;
        mcpwm_timer_t timer_num;
        mcpwm_config_t pwm_config;
    public:
        motorDC(/* args */);
        ~motorDC();
        esp_err_t init(mcpwm_unit_t mcpwm_num, mcpwm_io_signals_t io_signal, mcpwm_timer_t timer_num, int gpio_num_foward, int gpio_num_backward);
        esp_err_t setMaxVelocity(float vel);
        esp_err_t setVelocity(float vel);
};
