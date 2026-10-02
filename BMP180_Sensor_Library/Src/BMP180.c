/*
 * BMP180.c
 *
 *  Created on: Mar 23, 2025
 *      Author: Mert
 */

#include "BMP180.h"

static long BMP180_ReadUncTempVal(I2C_HandleTypeDef *hi2c);
static long BMP180_ReadUncPressVal(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc);


/* BMP180 FUNCTIONS */
int BMP180_ScanDeviceAddr(I2C_HandleTypeDef *hi2c)
{
	for(uint8_t address=0; address<255; address++)
	{
		if(HAL_I2C_IsDeviceReady(hi2c, address, TRIALS, TIMEOUT) == HAL_OK)
		{
			return address;
		}
	}
	return HAL_ERROR;
}

BMP180_ReadStatus_t BMP180_ReadData(I2C_HandleTypeDef *hi2c,  uint16_t RegAdd, uint8_t SizeOfData,uint8_t* DataBuf)
{
	if(HAL_I2C_Mem_Read(hi2c, BMP180_ADDR_READ, RegAdd, 1, DataBuf, SizeOfData, TIMEOUT) == HAL_OK)
	{
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

BMP180_WriteStatus_t BMP180_WriteData(I2C_HandleTypeDef *hi2c, uint16_t RegAdd, uint8_t SizeOfData, uint8_t* DataBuf)
{
	if(HAL_I2C_Mem_Write(hi2c, BMP180_ADDR_WRITE, RegAdd, 1, DataBuf, SizeOfData, TIMEOUT) == HAL_OK)
	{
		return WRITE_SUCCESS;
	}
	return WRITE_FAIL;
}

BMP180_ReadStatus_t BMP180_Read16Data(I2C_HandleTypeDef *hi2c,  uint16_t RegAdd, uint8_t SizeOfData, uint16_t* DataBuf)
{
	uint8_t temp[2] = {0};

	if(HAL_I2C_Mem_Read(hi2c, BMP180_ADDR_READ, RegAdd, I2C_MEMADD_SIZE_8BIT, temp, SizeOfData, TIMEOUT) == HAL_OK)
	{
		*DataBuf = (uint16_t)(temp[0] << 8 | temp[1]);
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

void BMP180_ReadCalibData(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc)
{
	BMP180_Read16Data(hi2c, CALIB_REG_AC1, 2, (uint16_t*)&calc.AC1);
	BMP180_Read16Data(hi2c, CALIB_REG_AC2, 2, (uint16_t*)&calc.AC2);
	BMP180_Read16Data(hi2c, CALIB_REG_AC3, 2, (uint16_t*)&calc.AC3);
	BMP180_Read16Data(hi2c, CALIB_REG_AC4, 2, (uint16_t*)&calc.AC4);
	BMP180_Read16Data(hi2c, CALIB_REG_AC5, 2, (uint16_t*)&calc.AC5);
	BMP180_Read16Data(hi2c, CALIB_REG_AC6, 2, (uint16_t*)&calc.AC6);
	BMP180_Read16Data(hi2c, CALIB_REG_B1, 2, (uint16_t*)&calc.B1);
	BMP180_Read16Data(hi2c, CALIB_REG_B2, 2, (uint16_t*)&calc.B2);
	BMP180_Read16Data(hi2c, CALIB_REG_MB, 2, (uint16_t*)&calc.MB);
	BMP180_Read16Data(hi2c, CALIB_REG_MC, 2, (uint16_t*)&calc.MC);
	BMP180_Read16Data(hi2c, CALIB_REG_MD, 2, (uint16_t*)&calc.MD);
}

static long BMP180_ReadUncTempVal(I2C_HandleTypeDef *hi2c)
{
	uint8_t data = 0x2E;
	uint16_t UT = 0;

	BMP180_WriteData(hi2c, BMP180_REG_CTRL_MEAS, 1, &data);
	HAL_Delay(5);
	BMP180_Read16Data(hi2c, BMP180_REG_OUT_MSB, 2, &UT);

	return UT;
}

static long BMP180_ReadUncPressVal(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc)
{

	uint8_t data = (0x34 + (calc.oss_val << 6));

	BMP180_WriteData(hi2c, BMP180_REG_CTRL_MEAS, 1, &data);
	HAL_Delay(5);
	uint8_t temp[3] = {0};
	long UP = 0;
	BMP180_ReadData(hi2c, BMP180_REG_OUT_MSB, 1, &temp[0]);
	BMP180_ReadData(hi2c, BMP180_REG_OUT_LSB, 1, &temp[1]);
	BMP180_ReadData(hi2c, BMP180_REG_OUT_XLSB, 1, &temp[2]);

	UP = ((temp[0] << 16) + (temp[1] << 8) + temp[2]) >> (8 - calc.oss_val);

	return UP;
}

float BMP180_GetTempVal(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc)
{
	// 1. Ham Sıcaklık Verisini Oku (UT)
	    uint8_t cmd = 0x2E;
	    uint8_t rx_buff[2] = {0};

	    // Sıcaklık ölçüm komutu gönder
	    HAL_I2C_Mem_Write(hi2c, (0x77 << 1), 0xF4, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
	    HAL_Delay(5); // Datasheet: Min 4.5ms bekleme süresi

	    // Ham veriyi oku (0xF6 MSB, 0xF7 LSB)
	    HAL_I2C_Mem_Read(hi2c, (0x77 << 1), 0xF6, I2C_MEMADD_SIZE_8BIT, rx_buff, 2, 100);

	    long UT = (long)((rx_buff[0] << 8) | rx_buff[1]);

	    // 2. Bosch Datasheet Hesaplama Algoritması
	    long x1 = ((UT - (long)calc.AC6) * (long)calc.AC5) >> 15;

	    // x1 ve MD toplamının 0 olma ihtimaline karşı güvenlik
	    if (x1 + calc.MD == 0) return 0.0f;

	    long x2 = ((long)calc.MC << 11) / (x1 + calc.MD);
	    long b5 = x1 + x2;

	    // Gerçek °C Sıcaklık Değeri (Float cinsinden)
	    float real_temperature = ((b5 + 8) >> 4) / 10.0f;

	    return real_temperature;
}

float BMP180_GetPressVal(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc)
{
	uint8_t ID_Buff = 0;

	BMP180_ReadData(hi2c, BMP180_REG_ID, 1, &ID_Buff);

	if(ID_Buff != 0x55)
	{
		//printf("HAL Error Code: %d\n", hi2c->ErrorCode);
		return HAL_ERROR;

	}

	long UP = BMP180_ReadUncPressVal(hi2c, calc);
	calc.P = 0;

	calc.B6 = calc.B5 - 4000;
	calc.X1 = (calc.B2 * (pow(calc.B6, 2)/pow(2,12)) / pow(2,11));
	calc.X2 = calc.AC2 * calc.B6 / pow(2,11);
	calc.X3 = calc.X1 + calc.X2;
	calc.B3 = ((((long)calc.AC1*4 + calc.X3) << calc.oss_val) + 2) / 4;
	calc.X1 = calc.AC3 * calc.B6 / pow(2,19);
	calc.X2 = (calc.B1 * (pow(calc.B6, 2) / pow(2,12)) / pow(2, 16));
	calc.X3 = ((calc.X1 + calc.X2) + 2) / pow(2,2);
	calc.B4 = (calc.AC4 * (unsigned long)(calc.X3 + 32768) / pow(2,15));
	calc.B7 = ((unsigned long)UP - calc.B3) * (50000 >> calc.oss_val);

	if(calc.B7 < 0x80000000)
	{
		calc.P = ((calc.B7 * 2) / calc.B4);
	}
	else
	{
		calc.P = ((calc.B7 / calc.B4) * 2);
	}

	calc.X1 = pow(calc.P/pow(2,8), 2);
	calc.X1 = (calc.X1 * 3038) / pow(2, 16);
	calc.X2 = (-7357 * calc.P) / pow(2,16);
	calc.P = calc.P + (calc.X1 + calc.X2 + 3791) / pow(2,4);

	return calc.P;
}
