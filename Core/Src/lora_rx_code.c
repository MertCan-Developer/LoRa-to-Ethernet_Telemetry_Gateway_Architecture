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
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w5500_lib.h"
#include "st7789.h"
#include "BMP180.h"
#include <stdio.h>
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LORA_UART_HANDLER	&huart1
#define BMP180_I2C_HANDLER	&hi2c1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// W5500 Variables
network_config net_conf;
uint8_t dev_ip_addr[4] = {192, 168, 1, 5};
uint8_t dev_mac_addr[6] = {170, 187, 204, 221, 238, 255};
uint8_t gateway_addr[4] = {192, 168, 1, 1};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t rtr_time = 250;
uint8_t rcr_val = 3;
uint8_t socket_status = 0;
char dest_ip_addr[11] = "192.168.1.8";
char dest_mac_addr[17] = "74 D4 DD 23 34 E8";

// ST7789 Screen Variables
char st7789_string_array_1[] = "W5500 Connected...";
char st7789_string_array_2[] = "W5500 Established...";
char st7789_string_array_3[] = "W5500 in UPD mode...";
char st7789_string_array_4[] = "Houston, we have a problem :)";
char st7789_string_array_5[] = "Destination IP:";
char st7789_string_array_6[] = "Destination MAC:";
char st7789_string_array_7[] = "Pressure:";
char st7789_string_array_8[] = "Temperature:";
char lora_character_arry[21] = {0};
uint8_t st7789_counter = 0;

// LoRa Variables
uint8_t rx_data[12] = {0};		// Received Data Buffer
uint8_t rx_byte = 0;			// Received Byte Data
volatile uint8_t rx_index = 0; 	// Received Byte Counter
volatile uint8_t rx_flag = 0;  	// Packet Completed Flag

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// LoRa Function Prototypes
void lora_send_data(char* data_to_send, uint8_t data_len);
void lora_wait_aux_ready(void);
HAL_StatusTypeDef lora_receive_data(char* rx_data, uint8_t data_len);

// Helper Function Prototypes
void press_num_to_char(uint8_t* press_pa, char* str);
void temp_num_to_char(uint8_t* temp_c, char* str);


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
	net_conf.device_ip_addr = dev_ip_addr;
	net_conf.device_mac_addr = dev_mac_addr;
	net_conf.gateway_addr = gateway_addr;
	net_conf.subnet_mask = subnet_mask;

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
  w5500_init(net_conf, rtr_time, rcr_val, W5500_SPI_OM_VDM);
  w5500_socket_tcp_init(w5500_socket_5, w5500_tcp_client);
  HAL_Delay(3000);
  socket_status = w5500_read_socket_status(w5500_socket_5);

  st7789_init();
  st7789_fill_the_screen(ST7789_COLOR_WHITE);


  if(socket_status == 0x13)			// Connected TCP
  {
	  st7789_write_string(5, 5, st7789_string_array_1, 20, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
  }
  else if(socket_status == 0x17)	// Established
  {
	  st7789_write_string(3, 5, st7789_string_array_2, 18, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
	  st7789_write_string(3, 35, st7789_string_array_5, sizeof(st7789_string_array_5), ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
	  st7789_write_string(3, 53, dest_ip_addr, 11, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
	  st7789_write_string(3, 83, st7789_string_array_6, sizeof(st7789_string_array_6), ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
	  st7789_write_string(3, 101, dest_mac_addr, 17, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
  }
  else if(socket_status == 0x22)	// UDP mode
  {
	  st7789_write_string(3, 5, st7789_string_array_3, sizeof(st7789_string_array_3), ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
  }
  else	// Houston, we have a problem
  {
	  st7789_write_string(3, 5, st7789_string_array_4, 9, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
	  st7789_write_string(3, 23, st7789_string_array_4 + 9, sizeof(st7789_string_array_4) - 9, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
  }

  __HAL_UART_CLEAR_OREFLAG(&huart1);
  __HAL_UART_CLEAR_NEFLAG(&huart1);
  __HAL_UART_CLEAR_FEFLAG(&huart1);

  HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
  st7789_write_string(3, 140, st7789_string_array_7, sizeof(st7789_string_array_7), ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
  st7789_write_string(3, 160, st7789_string_array_8, sizeof(st7789_string_array_8), ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	      if (rx_flag == 1)
	      {
	          // Ekrana bas
	    	  if(st7789_counter < 2) st7789_counter++;
	    	  if(st7789_counter == 2){
	    		  st7789_erase_string(120, 140, 11, ST7789_COLOR_WHITE);
	    		  st7789_counter++;
	    	  }

	    	  press_num_to_char(rx_data, lora_character_arry);
	    	  temp_num_to_char(rx_data, lora_character_arry);
	          st7789_write_string(110, 140, lora_character_arry, 11, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);
	          st7789_write_string(150, 160, &lora_character_arry[11], 9, ST7789_COLOR_BLACK, ST7789_COLOR_WHITE);

	          if(socket_status == 0x17)
	          {
	        	  w5500_tcp_transmit((uint8_t*)lora_character_arry, 11, w5500_socket_5);
	        	  w5500_tcp_transmit((uint8_t*)&lora_character_arry[7], 1, w5500_socket_5);
	        	  w5500_tcp_transmit((uint8_t*)&lora_character_arry[7], 1, w5500_socket_5);
	        	  w5500_tcp_transmit((uint8_t*)&lora_character_arry[7], 1, w5500_socket_5);
	        	  w5500_tcp_transmit((uint8_t*)&lora_character_arry[11], 9, w5500_socket_5);
	          }
	          else if(socket_status == 0x22)
	          {
	        	  w5500_udp_transmit((uint8_t*)lora_character_arry, 11, w5500_socket_5);
	        	  w5500_udp_transmit((uint8_t*)&lora_character_arry[7], 1, w5500_socket_5);
	        	  w5500_tcp_transmit((uint8_t*)&lora_character_arry[7], 1, w5500_socket_5);
	        	  w5500_tcp_transmit((uint8_t*)&lora_character_arry[7], 1, w5500_socket_5);
	        	  w5500_udp_transmit((uint8_t*)&lora_character_arry[11], 9, w5500_socket_5);
	          }

	          // Reset the flag and the index for the next packet
	          rx_index = 0;
	          rx_flag = 0;
	      }

  /* USER CODE END 3 */
  }
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void press_num_to_char(uint8_t* press_pa, char* str)
{
	    uint32_t press_pascal = ((uint32_t)press_pa[2] << 24) |
	                            ((uint32_t)press_pa[3] << 16) |
	                            ((uint32_t)press_pa[4] << 8)  |
	                             (uint32_t)press_pa[5];

	    uint8_t d1 = (press_pascal / 100000) % 10;
	    uint8_t d2 = (press_pascal / 10000)  % 10;
	    uint8_t d3 = (press_pascal / 1000)   % 10;
	    uint8_t d4 = (press_pascal / 100)    % 10;

	    uint8_t d5 = (press_pascal / 10)     % 10;
	    uint8_t d6 =  press_pascal           % 10;


	    str[0]  = d1 + '0';
	    str[1]  = d2 + '0';
	    str[2]  = d3 + '0';
	    str[3]  = d4 + '0';
	    str[4]  = '.';
	    str[5]  = d5 + '0';
	    str[6]  = d6 + '0';
	    str[7]  = ' ';
	    str[8]  = 'h';
	    str[9]  = 'P';
	    str[10] = 'a';
}

void temp_num_to_char(uint8_t* temp_c, char* str)
{

	uint8_t d1 = (temp_c[0] / 10) % 10;
	uint8_t d2 = temp_c[0]   % 10;
	uint8_t d3 = (temp_c[1] / 10) % 10;
	uint8_t d4 = temp_c[1]   % 10;

	str[11] = d1 + 48;
	str[12] = d2 + 48;
	str[13] = '.';
	str[14] = d3 + 48;
	str[15] = d4 + 48;
	str[16] = ' ';
	str[17] = 'C';
	str[18] = '\r';
	str[19] = '\n';
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // If the previous packet doesn't processed at main loop then don't write the new data to the buffer
        if (rx_flag == 0)
        {
            rx_data[rx_index] = rx_byte;
            rx_index++;

            // Complete the packet when the packet would be 6 byte
            if (rx_index >= 6)
            {
                rx_flag = 1; // Inform the main loop that the packet is ready
            }
        }

        // Bir sonraki baytı almak için kesmeyi TEKRAR BAŞLAT
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);

        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
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
