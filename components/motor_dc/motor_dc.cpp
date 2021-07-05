#include <stdio.h>
#include <typeinfo>
#include "motor_dc.h"



motorDC::motorDC()
{
}

esp_err_t motorDC::init(mcpwm_unit_t _mcpwm_num, mcpwm_io_signals_t io_signal_A,
                        mcpwm_timer_t _timer_num, int gpio_num_foward,
                        int gpio_num_backward)
{
    if (io_signal_A == MCPWM0A)
    {
        io_signals_foward = MCPWM0A;
        io_signals_backward = MCPWM0B;
    }
    if (io_signal_A == MCPWM1A)
    {
        io_signals_foward = MCPWM1A;
        io_signals_backward = MCPWM1B;
    }
    if (io_signal_A == MCPWM2A)
    {
        io_signals_foward = MCPWM2A;
        io_signals_backward = MCPWM2B;
    }

    pin_foward = gpio_num_foward;
    pin_backward = gpio_num_backward;
    mcpwm_num = _mcpwm_num;
    timer_num = _timer_num;
    mcpwm_gpio_init(mcpwm_num, io_signals_foward, pin_foward);
    mcpwm_gpio_init(mcpwm_num, io_signals_backward, pin_backward);
    pwm_config.frequency = 20000;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    return mcpwm_init(mcpwm_num, timer_num, &pwm_config);
}

esp_err_t motorDC::setMaxVelocity(float vel)
{
    max_velocity = vel;
    return ESP_OK;
}

esp_err_t motorDC::setVelocity(float vel)
{
    if (vel == 0)
    { //stop
        mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
        mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
        return ESP_OK;
    }
    if (vel > 0)
    { //foward
        if (vel > max_velocity)
        {
            vel = max_velocity;
        }
        float duty_cycle = 100 * vel / max_velocity;
        mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
        return ESP_OK;
    }
    else
    { //backward
        vel = -vel;
        if (vel < max_velocity)
        {
            vel = max_velocity;
        }
        float duty_cycle = 100 * vel / max_velocity;
        mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
        return ESP_OK;
    }
}

motorDC::~motorDC()
{
    //TODO
}