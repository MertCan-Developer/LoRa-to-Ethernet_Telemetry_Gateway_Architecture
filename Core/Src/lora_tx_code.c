/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w5500_lib.h"
#include "st7789.h"
#include "BMP180.h"
#include "i2c.h"
#include <stdio.h>
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BMP180_I2C_HANDLER  &hi2c1
#define LORA_UART_HANDLER	&huart1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// LoRa Variables
char rx_data[100] = {0};
uint8_t data_to_send[15] = {0};


// BMP180 Variables
uint32_t temp_val = 0;
uint32_t press_val_pascal = 0;
uint8_t bmp180_temp_mantissa = 0;
uint8_t bmp180_temp_fraction = 0;
uint32_t press_val_hectoPascal = 0;
uint8_t press_msb_2 = 0;
uint8_t press_msb_1 = 0;
uint8_t press_lsb_2 = 0;
uint8_t press_lsb_1 = 0;
uint8_t bmp180_temp_press_buff[6] = {0};
BMP180_Coef_t calc;
uint32_t press_pascal = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// LoRa Private Function Prototypes
void lora_send_data(char* data_to_send, uint8_t data_len);
void lora_wait_aux_ready(void);
void lora_receive_data(char* rx_data, uint8_t data_len);

// BMP180 Private Function Prototypes
void bmp180_get_calib_coeff(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  bmp180_get_calib_coeff();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  temp_val = (uint32_t)BMP180_GetTempVal(&hi2c1, calc);
	  bmp180_temp_mantissa = (uint8_t)temp_val;
	  bmp180_temp_fraction = ((uint8_t)(temp_val - (uint32_t)bmp180_temp_mantissa) * 100);

	      press_val_pascal = (uint32_t)BMP180_GetPressVal(&hi2c1, calc);

	      uint8_t press_msb_2 = (press_pascal >> 24) & 0xFF;
	      uint8_t press_msb_1 = (press_pascal >> 16) & 0xFF;
	      uint8_t press_lsb_2 = (press_pascal >> 8)  & 0xFF;
	      uint8_t press_lsb_1 =  press_pascal        & 0xFF;

	      bmp180_temp_press_buff[0] = bmp180_temp_mantissa;
	      bmp180_temp_press_buff[1] = bmp180_temp_fraction;

	      bmp180_temp_press_buff[2] = press_msb_2;
	      bmp180_temp_press_buff[3] = press_msb_1;
	      bmp180_temp_press_buff[4] = press_lsb_2;
	      bmp180_temp_press_buff[5] = press_lsb_1;

	      lora_send_data((char*)bmp180_temp_press_buff, 6);
	      HAL_Delay(1000);}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void bmp180_get_calib_coeff(void)
{
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_AC1, 2, (uint16_t*)&calc.AC1);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_AC2, 2, (uint16_t*)&calc.AC2);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_AC3, 2, (uint16_t*)&calc.AC3);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_AC4, 2, (uint16_t*)&calc.AC4);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_AC5, 2, (uint16_t*)&calc.AC5);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_AC6, 2, (uint16_t*)&calc.AC6);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_B1, 2, (uint16_t*)&calc.B1);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_B2, 2, (uint16_t*)&calc.B2);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_MB, 2, (uint16_t*)&calc.MB);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_MC, 2, (uint16_t*)&calc.MC);
	BMP180_Read16Data(BMP180_I2C_HANDLER, CALIB_REG_MD, 2, (uint16_t*)&calc.MD);
}

// If the AUX pin of the LoRa module in LOW state it means that the module is busy
// We need to wait until the AUX pin gets the HIGH state
void lora_wait_aux_ready(void)
{
	uint8_t lora_gpio_pin = HAL_GPIO_ReadPin(LORA_AUX_GPIO_Port, LORA_AUX_Pin);

	while(lora_gpio_pin == GPIO_PIN_RESET)
	{
		HAL_Delay(1);
	}
}

void lora_send_data(char* data_to_send, uint8_t data_len)
{
	lora_wait_aux_ready();

	HAL_UART_Transmit(LORA_UART_HANDLER, data_to_send, data_len, 1000);
}

void lora_receive_data(char* rx_data, uint8_t data_len)
{
	lora_wait_aux_ready();

	HAL_UART_Receive(LORA_UART_HANDLER, rx_data, data_len, 1000);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
