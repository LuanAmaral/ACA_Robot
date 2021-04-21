#include <stdio.h>

#include "motor_dc.h"
// #include "i2c.h"
#include "GY80.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define PWMA 12U
#define PWMB 13U
#define I2C_DATA 2U
#define I2C_CLOCK 14U
#define I2C_FREQ 5000

#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif
extern "C" void app_main()
{
    printf("Testing GY80...\n");
    i2c i2c;
    i2c.init(I2C_DATA, I2C_CLOCK,I2C_FREQ, I2C_NUM_0 );
    GY80 gy80(i2c);
    printf("Leitura:\tacc_x\tacc_y\tacc_z\tgyr_x\tgyr_y\tgyr_z ");
    while (1)
    {
        gy80.ReadAcc();
        gy80.ReadGyr();
        printf("\t%d\t%d\t%d\t%d\t%d\t%d", gy80.Imu.Acc.X,gy80.Imu.Acc.Y,gy80.Imu.Acc.Z,gy80.Imu.Gyr.X,gy80.Imu.Gyr.Y,gy80.Imu.Gyr.Z);
        vTaskDelay(100/portTICK_PERIOD_MS);

        
    }

    
}
