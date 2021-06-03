
/*
 * MPU6050.cpp
 *
 *  Created on: 30 oct 2020
 *      Author: Luan Amaral
 */

#include <stdio.h>
#include "MPU6050.h"

#include "MPU6050.h"
#include <stdio.h>
#include <typeinfo>

esp_err_t MPU6050::I2CRead(MPU_DEVICE_ADDR device_addr, uint8_t reg, uint8_t *buffer, size_t size)
{
    return i2c_module.read(device_addr, reg, buffer, size);
}

esp_err_t MPU6050::I2CWrite(MPU_DEVICE_ADDR device_addr, uint8_t reg, uint8_t *buffer, size_t data_size)
{
    return i2c_module.write(device_addr, reg, buffer, data_size);
}

bool MPU6050::WaitDataReady()
{
    uint8_t rx = 0;
    I2CRead(MPU_ADDR, MPU_INTSTATUS, &rx, 1);
    if ((rx & 0b00000001) == 0b00000001)
        return true;
    return false;
}

MPU6050::MPU6050(i2c _i2c_module)
{
    i2c_module = _i2c_module;
}

bool MPU6050::Init()
{
    uint8_t rx = 0;
    DeviceConnected = false;
    I2CRead(MPU_ADDR, MPU_WHOAMI, &rx, 1);
    if (rx != 0x68)
    {
        return false;
    }
    // All devices responded :)
    uint8_t tx;

    /*Accelerometer Configuration*/
    tx = 00;
    I2CWrite(MPU_ADDR, MPU_SAMPLERATE, &tx, 1);

    tx = 0b00000000;
    I2CWrite(MPU_ADDR, MPU_CONFIG, &tx, 1);

    tx = 0b00000000; //ACC 400Hz
    I2CWrite(MPU_ADDR, MPU_PWRMGMT, &tx, 1);

    tx = 0b00001000; //FULL SCALE 500 DEGREES/SEC
    I2CWrite(MPU_ADDR, MPU_GYR_CONFIG, &tx, 1);

    tx = 0b00001000; //FULL SCALE 4g
    I2CWrite(MPU_ADDR, MPU_ACC_CONFIG, &tx, 1);

    DeviceConnected = true;

    return true;
}

void MPU6050::ReadAcc()
{
    // while (WaitDataReady() == false);
    uint8_t buffer[6];
    I2CRead(MPU_ADDR, MPU_ACC_DATAX, buffer, 6);
    Imu.Acc.X = (int16_t) ((buffer[1] << 8) | buffer[0]);
	Imu.Acc.Y = (int16_t) ((buffer[3] << 8) | buffer[2]);
	Imu.Acc.Z = (int16_t) ((buffer[5] << 8) | buffer[4]);
}

void MPU6050::ReadGyr()
{
    //while(WaitDataReady() == false);
    uint8_t buffer[6];
    I2CRead(MPU_ADDR, MPU_GYR_DATAX, buffer, 6);

    Imu.Gyr.X = (int16_t) ((buffer[1] << 8) | buffer[0]);
	Imu.Gyr.Y = (int16_t) ((buffer[3] << 8) | buffer[2]);
	Imu.Gyr.Z = (int16_t) ((buffer[5] << 8) | buffer[4]);
}

void MPU6050::ReadTemp()
{
    while (WaitDataReady() == false);
    uint8_t buffer[1];
    I2CRead(MPU_ADDR, MPU_TEM_DATA, buffer, 1);
    Imu.Temp = buffer[1];
}

void MPU6050::ReadAll()
{
    ReadAcc();
    ReadGyr();
}

bool MPU6050::isDeviceConnected()
{
    return DeviceConnected;
}