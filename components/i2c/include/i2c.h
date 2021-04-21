/*
 *  i2c.h
 *
 *  Created on: 16 jan 2021
 *      Author: Luan Amaral
 */

#ifndef i2c_H_
#define i2c_H_
#endif

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_attr.h"

#define I2C_MASTER_SCL_IO 14 /*!< gpio number for i2c slave clock */
#define I2C_MASTER_SDA_IO 15 /*!< gpio number for i2c slave data */
#define I2C_ACK  (i2c_ack_type_t)0X01      /*!< I2C master will check ack flom slave*/
#define I2C_NACK (i2c_ack_type_t)0X00      /*!< I2C master will not check ack flom slave*/
#define I2C_MASTER_FREQ_HZ 100

class i2c
{
    private:
        //int port;
        i2c_config_t conf;

    public:
        size_t rx_buff = 0; /*!<disable */
        size_t tx_buff = 0; /*!<disable */
        i2c_port_t port;

        i2c();
        ~i2c();
        esp_err_t init(int data_pin, int clock_pin, uint32_t freq, i2c_port_t _port);
        esp_err_t write(uint8_t adress, uint8_t regist, uint8_t *data, size_t size);
        esp_err_t read(uint8_t address, uint8_t regist, uint8_t *data, size_t size);
};