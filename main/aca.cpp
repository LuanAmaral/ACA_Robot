#include <stdio.h>

#include "motor_dc.h"
// #include "i2c.h"
#include "GY80.h"
#include "encoder.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "driver/gpio.h"


#define ENCODER_A (gpio_num_t)2
#define ENCODER_B (gpio_num_t)14
#define I2C_FREQ  (uint32_t)1000
#define BLINK_GPIO (gpio_num_t)4


#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif
extern "C" void app_main()
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO, 0);
    printf("Testing Encoder...\n");
    encoder ky40 (ENCODER_A, ENCODER_B, 20, PCNT_CHANNEL_0, PCNT_UNIT_0);
    float distancia_rad;
    while(1)
    {
        ky40.get_dist_angular_rad(&distancia_rad);
        printf("%f\n", distancia_rad);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    

    

}
