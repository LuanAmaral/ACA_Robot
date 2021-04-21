/*
 *  gy80.h
 *
 *  Created on: 14 apr 2021
 *      Author: Luan Amaral
 */

#ifndef GY80_H_
#define GY80_H_


#include "i2c.h"

#define I2C_GY_MODULE I2C1

/*Accelerometer Registers*/
#define GY_ACC_WHOAMI 0x00
#define GY_ACC_BWRATE 0x2C
#define GY_ACC_POWERCTL 0x2D
#define GY_ACC_INTENABLE 0x2E
#define GY_ACC_INTSOURCE 0x30
#define GY_ACC_DATAFORMAT 0x31
#define GY_ACC_DATAREG 0x32

/*Gyroscope Registers*/
#define GY_GYR_WHOAMI_REG 0x0F
#define GY_GYR_CTROL_REG1 0x20
#define GY_GYR_CTROL_REG2 0x21
#define GY_GYR_CTROL_REG3 0x22
#define GY_GYR_CTROL_REG4 0x23
#define GY_GYR_CTROL_REG5 0x24
#define GY_GYR_OUT_TEMP_REG 0x26
#define GY_GYR_STATUS_REG 0x27
#define GY_GYR_DATA_REG 0xA8 //Actual register is 0x28, but MSB bit is needed to indicate auto-increment (-_-)

/*Magnetometer Registers*/
#define GY_MAG_CONFIG_A_REG 0x00
#define GY_MAG_CONFIG_B_REG 0x01
#define GY_MAG_MODE_REG 0x02
#define GY_MAG_DATA_REG 0x03
#define GY_MAG_STATUS_REG 0x09
#define GY_MAG_WHOAMI_REG 0x0A

/*General Parameters*/
#define GY_GYR_RESOLUTION 17.5e-3 // 17.5e-3 deg/LSB
#define GY_ACC_RESOLUTION 4e-3 //4mg / LSB
#define GY_MAG_RESOLUTION 0.92e-3 //0.92 mG/LSb

class GY80 {

	//Endereço dos dispositivos
	enum GY_DEVICE_ADDR {
		ACC_ADDR = 0x53,
		GYR_ADDR = 0x69,
		MAG_ADDR = 0x1E
	};

	//Vetor
	struct Ivector3 {
		int16_t X;
		int16_t Y;
		int16_t Z;
	};

private:
    i2c i2c_module;
	esp_err_t I2CRead(GY_DEVICE_ADDR device_addr, uint8_t reg,uint8_t* buffer,size_t data_size);
	esp_err_t I2CWrite(GY_DEVICE_ADDR device_addr, uint8_t reg,uint8_t* buffer,size_t data_size);

	bool WaitGyrDataReady();
	bool WaitAccDataReady();
	bool WaitMagDataReady();
	bool DeviceConnected = false;


public:
	//Recebe os dados e calibrações
	struct IMUData {
		Ivector3 Acc;
		Ivector3 Mag;
		Ivector3 Gyr;

		Ivector3 AccOffset;
		Ivector3 GyroOffset;
		Ivector3 MagOffset;
	}Imu;

	GY80(i2c _i2c_modules);
	bool Init();
	void ReadAcc();
	void ReadGyr();
	void ReadMag();
	void ReadAll();
	bool isDeviceConnected();

	void AutoCalibrateMag();

	friend class IMU;

};

#endif