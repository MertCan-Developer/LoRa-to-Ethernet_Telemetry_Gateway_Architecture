/*
 * BMP180.h
 *
 *  Created on: Mar 23, 2025
 *      Author: Mert
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "main.h"
#include "math.h"

#define BMP180_ADDR_WRITE	0xEE
#define BMP180_ADDR_READ	0xEF
#define TIMEOUT		1000
#define	TRIALS		3

/*Coefficients Structure*/
typedef struct
{
	short AC1;
	short AC2;
	short AC3;
	unsigned short AC4;
	unsigned short AC5;
	unsigned short AC6;
	short B1;
	short B2;
	short MB;
	short MC;
	short MD;

	long X1;
	long X2;
	long X3;
	long B3;
	unsigned long B4;
	long B5;
	long B6;
	unsigned long B7;
	long T;
	long P;
	short oss_val;
}BMP180_Coef_t;


/*Register Address' of Device*/
#define BMP180_REG_SOFT			0xE0
#define BMP180_REG_CTRL_MEAS	0xF4
#define BMP180_REG_OUT_MSB		0xF6
#define BMP180_REG_OUT_LSB		0xF7
#define BMP180_REG_OUT_XLSB		0xF8
#define BMP180_REG_ID			0xD0

/*Register Address' of Calibration Values*/
#define CALIB_REG_AC1 			0xAA
#define CALIB_REG_AC2			0xAC
#define CALIB_REG_AC3 			0xAE
#define CALIB_REG_AC4			0xB0
#define CALIB_REG_AC5 			0xB2
#define CALIB_REG_AC6			0xB4
#define CALIB_REG_B1	 		0xB6
#define CALIB_REG_B2			0xB8
#define CALIB_REG_MB	 		0xBA
#define CALIB_REG_MC			0xBC
#define CALIB_REG_MD	 		0xBE

typedef enum
{
	READ_FAIL = 0,
	READ_SUCCESS
}BMP180_ReadStatus_t;

typedef enum
{
	WRITE_FAIL = 0,
	WRITE_SUCCESS
}BMP180_WriteStatus_t;

typedef enum
{
	INIT_FAIL = 0,
	INIT_SUCCESS
}BMP180_InitStatus_t;


/* Over Sampling Ratio's */
#define OSS_SINGLE			0
#define OSS_TWO_TIMES		1
#define OSS_FOUR_TIMES		2
#define OSS_EIGHT_TIMES		8

int BMP180_ScanDeviceAddr(I2C_HandleTypeDef *hi2c);
BMP180_ReadStatus_t BMP180_ReadData(I2C_HandleTypeDef *hi2c,  uint16_t RegAdd, uint8_t SizeOfData, uint8_t* DataBuf);
BMP180_ReadStatus_t BMP180_Read16Data(I2C_HandleTypeDef *hi2c,  uint16_t RegAdd, uint8_t SizeOfData, uint16_t* DataBuf);
BMP180_WriteStatus_t BMP180_WriteData(I2C_HandleTypeDef *hi2c, uint16_t RegAdd, uint8_t SizeOfData, uint8_t* DataBuf);
float BMP180_GetTempVal(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc);
float BMP180_GetPressVal(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc);
void BMP180_ReadCalibData(I2C_HandleTypeDef *hi2c, BMP180_Coef_t calc);

#endif /* INC_BMP180_H_ */
