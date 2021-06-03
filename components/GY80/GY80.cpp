/*
 *  gy80.cpp
 *
 *  Created on: 14 apr 2021
 *      Author: Luan Amaral
 */

#include <stdio.h>
#include "GY80.h"

esp_err_t  GY80::I2CRead(GY_DEVICE_ADDR device_addr, uint8_t reg,uint8_t* buffer, size_t size){
	return i2c_module.read(device_addr, reg, buffer, size);
}

esp_err_t  GY80::I2CWrite(GY_DEVICE_ADDR device_addr,uint8_t reg,uint8_t* buffer,size_t data_size){
	return i2c_module.write(device_addr, reg, buffer, data_size);

}

bool GY80::WaitAccDataReady(){
	uint8_t rx = 0;
	I2CRead(ACC_ADDR, GY_ACC_INTSOURCE, &rx, 1);
	if( (rx&0b10000000) == 0b10000000)
		return true;
	return false;
}

bool GY80::WaitGyrDataReady(){
	uint8_t rx = 0;
	I2CRead(GYR_ADDR, GY_GYR_STATUS_REG, &rx, 1);
	if( (rx&0b00001000) == 0b00001000)
		return true;
	return false;
}

bool GY80::WaitMagDataReady(){
	uint8_t rx = 0;
	I2CRead(MAG_ADDR, GY_MAG_STATUS_REG, &rx, 1);
	if( (rx&0b00000001) == 0b00000001)
		return true;
	return false;
}

GY80::GY80(i2c _i2c_module) {
	// TODO Auto-generated constructor stub
//	Init(); 
    i2c_module = _i2c_module;
}

bool GY80::Init(){
	uint8_t rx = 0;
	DeviceConnected = false;
		I2CRead(ACC_ADDR, GY_ACC_WHOAMI, &rx, 1);
		if(rx != 0b11100101)
		{
			printf("ACC %d\n", rx);
			return ESP_FAIL;
		}

		I2CRead(GYR_ADDR, GY_GYR_WHOAMI_REG, &rx, 1);
		if(rx != 0b11010011)
		{
			printf("GYR\n");
			return ESP_FAIL;
		}

		I2CRead(MAG_ADDR, GY_MAG_WHOAMI_REG, &rx, 1);
		if(rx != 0b01001000)
		{
			printf("MAG\n");
			return ESP_FAIL;
		}
		// All devices responded :)
		uint8_t tx;

		/*Accelerometer Configuration*/
		tx = 0b00001010; //NORMAL POWER, 100 Hz
		I2CWrite(ACC_ADDR, GY_ACC_BWRATE, &tx,1);

		tx = 0b00001000; //NORMAL MEASUREMENT ON
		I2CWrite(ACC_ADDR, GY_ACC_POWERCTL, &tx,1);

		tx = 0b00001001; //FULL RESOLUTION, +-4g , 4mg/LSB. 14-bit
		I2CWrite(ACC_ADDR, GY_ACC_DATAFORMAT, &tx,1);


		/*Gyroscope Configuration*/
		tx = 0b00001111; // POWER ON, 3 AXIS ENABLE
		I2CWrite(GYR_ADDR, GY_GYR_CTROL_REG1, &tx,1);

	//	tx = 0b00000000; //HP FILTER @ 8 Hz CUT-OFF

		tx = 0b00010000; // CONTINOUS, LITTLE ENDIAN, +- 500dps
		I2CWrite(GYR_ADDR, GY_GYR_CTROL_REG4, &tx,1);

		tx = 0b00000000; //HIGH-PASS DISABLED
		I2CWrite(GYR_ADDR, GY_GYR_CTROL_REG5, &tx,1);

		/*Magnetometer Configuration*/
		tx = 0b00000000; //no avg, no measruement bias
		I2CWrite(MAG_ADDR, GY_MAG_CONFIG_A_REG, &tx, 1);

		tx = 0b00100000; // +-1.3 Gauss (1gauss = 100uT)
		I2CWrite(MAG_ADDR, GY_MAG_CONFIG_B_REG, &tx, 1);

	//	tx = 0b00000000; //Single-Measure (up to 160Hz)
	//	GY_I2C_WRITE(GY_DEVICE_MAGN, GY_MAG_MODE_REG, &tx, 1);
		/*READY! */
		DeviceConnected = true;

		return ESP_OK;
}

/* IMPORTANTE --> Alinhamento dos eixos eh feito na hora da leitura! */
//BODY X = - GY_Y
//BODY Y = - GY_Z
//BODY Z = GY_X


void GY80::ReadAcc(){
	// while(WaitAccDataReady() == false);
	uint8_t buffer[6]; //6 data bytes
	I2CRead(ACC_ADDR, GY_ACC_DATAREG, buffer, 6);
	Imu.Acc.Z = (int16_t) ((buffer[1] << 8) | buffer[0]);
	Imu.Acc.X = (int16_t) ((buffer[3] << 8) | buffer[2]);
	Imu.Acc.Y = (int16_t) ((buffer[5] << 8) | buffer[4]);
}

void GY80::ReadGyr(){
	// while(WaitGyrDataReady() == false);
	uint8_t buffer[6]; //6 data bytes
	I2CRead(GYR_ADDR, GY_GYR_DATA_REG, buffer, 6);
//	GY_I2C_READ(GY_DEVICE_GYRO, 0x29, &buffer[1], 1);
	Imu.Gyr.Z = (int16_t) ((buffer[1] << 8) | buffer[0]);
	Imu.Gyr.X = (int16_t) ((buffer[3] << 8) | buffer[2]);
	Imu.Gyr.Y = (int16_t) ((buffer[5] << 8) | buffer[4]); //pitch invertido
}

void GY80::ReadMag(){
	uint8_t tx = 0b00000001; //Single-Measure
	I2CWrite(MAG_ADDR, GY_MAG_MODE_REG, &tx, 1);
	while( WaitMagDataReady() == false);
	uint8_t buffer[6];
	I2CRead(MAG_ADDR, GY_MAG_DATA_REG, buffer, 6);
	Imu.Mag.Z = (int16_t) ((buffer[0] << 8) | buffer[1]);
	Imu.Mag.X = (int16_t) ((buffer[2] << 8) | buffer[3]);
	Imu.Mag.Y = (int16_t) ((buffer[4] << 8) | buffer[5]);
}

void GY80::AutoCalibrateMag(){
	static int Max_x = 0;
	static int Max_y = 0;
	static int Max_z = 0;
	static int Min_x = 0;
	static int Min_y = 0;
	static int Min_z = 0;

	if(Imu.Mag.X > Max_x)
		Max_x = Imu.Mag.X;
	if(Imu.Mag.X < Min_x)
			Min_x = Imu.Mag.X;

	if(Imu.Mag.Y > Max_y)
		Max_y = Imu.Mag.Y;
	if(Imu.Mag.Y < Min_y)
			Min_y = Imu.Mag.Y;

	if(Imu.Mag.Z > Max_z)
		Max_z = Imu.Mag.Z;
	if(Imu.Mag.Z < Min_z)
			Min_z = Imu.Mag.Z;

	Imu.MagOffset.X = (Max_x+Min_x)/2;
	Imu.MagOffset.Y = (Max_y+Min_y)/2;
	Imu.MagOffset.Z = (Max_z+Min_z)/2;
}

void GY80::ReadAll(){
	ReadAcc();
	ReadMag();
	ReadGyr();
}

bool GY80::isDeviceConnected(){
	return DeviceConnected;
}