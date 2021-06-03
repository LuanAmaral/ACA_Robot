#include <stdio.h>

// #include "motor_dc.h"
// #include "i2c.h"
#include "GY80.h"
// #include "MPU6050.h"
// #include "encoder.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "driver/gpio.h"

#define DATA_I2C   (gpio_num_t)21
#define CLOCK_I2C  (gpio_num_t)22
#define FREQ_I2C    500000

#define BLINK_GPIO (gpio_num_t)2

#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif
extern "C" void app_main()
{
    printf("Test GY80.h\n");
    i2c i2c_module;
    ESP_ERROR_CHECK(i2c_module.init(DATA_I2C, CLOCK_I2C, FREQ_I2C, I2C_NUM_0));

    printf("i2c module ok\n");
    GY80 imu_sensor(i2c_module);
    ESP_ERROR_CHECK(imu_sensor.Init());

    printf("gy80 initialized\n");
    while (1)
    {
        printf("Read:\n");
        imu_sensor.ReadAcc();
        imu_sensor.ReadGyr();
        printf("%d\t", imu_sensor.Imu.Acc.X);
        printf("%d\t", imu_sensor.Imu.Acc.Y);
        printf("%d\t", imu_sensor.Imu.Acc.Z);
        printf("%d\t", imu_sensor.Imu.Gyr.X);
        printf("%d\t", imu_sensor.Imu.Gyr.Y);
        printf("%d\n", imu_sensor.Imu.Gyr.Z);
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
}
