/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "rc522.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */
uint8_t text1[] = "kakha!\n\r";
uint8_t blockAddr;
uint8_t RC_size;

uint8_t sectorKeyA[16][16] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};

uint8_t status;
uint8_t	str[MFRC522_MAX_LEN];
uint8_t	str1[128];

uint8_t sn[4];
uint8_t buff[64];
uint8_t buff1[64];
int cardRead = 0;
int cardRead1 = 0;
int cardRead2 = 0;

int8_t i;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t MFRC522_Check(uint8_t *id);
uint8_t MFRC522_Compare(uint8_t *CardID, uint8_t *CompareID);
void MFRC522_WriteRegister(uint8_t addr, uint8_t val);
uint8_t MFRC522_ReadRegister(uint8_t addr);
void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);
void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen,
		uint8_t *backData, uint16_t *backLen);
uint8_t MFRC522_Anticoll(uint8_t *serNum);
void MFRC522_CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);
uint8_t MFRC522_SelectTag(uint8_t *serNum);
uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey,
		uint8_t *serNum);
uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData);
uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData);
void MFRC522_Init(void);
void MFRC522_Reset(void);
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
void MFRC522_Halt(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void led(uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);    // LED1 ON
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // LED1 OFF
		HAL_Delay(100);
	}
}
uint8_t * generate_random() { //this will generate random number in range 55 to 126 than converted to ASCII
    uint8_t i;
    static uint8_t chars[20];

    srand(HAL_GetTick());

   for (i = 0; i < 16; i++) {
      uint8_t rand_num = (rand() % (126 - 55 + 1)) + 55;
      chars[i] = rand_num;
   }
    return chars;

}
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	MFRC522_Init();
	led(3);

  /* USER CODE END 2 */
	cardRead = 0;
	cardRead1 = 0;
	cardRead2 = 0;

	uint8_t *p;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  HAL_Delay(10);
	  //sprintf((char*) buff, "ID: %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\r\n", (p+1));

	  //HAL_UART_Transmit(&huart2, p, 16, 200);

	  while(cardRead1 == 0){
	   	MFRC522_Init();
	   		if (!MFRC522_Request(PICC_REQIDL, str)) {
	   			if (!MFRC522_Anticoll(str)) {
	   				sprintf((char*) buff, "ID: \"%x%x%x%x\" ", str[0], str[1],str[2], str[3]);
	   				HAL_UART_Transmit(&huart2, buff, sizeof buff/sizeof buff[0], 200);
	   				cardRead1 = 1;


	   			}
	   		}

	   }

p = generate_random();

	  HAL_Delay(20);
	  MFRC522_Init();
	while(cardRead == 0)	{
			MFRC522_Init();
			status = MFRC522_Request(PICC_REQIDL, str);
			if (status == MI_OK) {
				status = MFRC522_Anticoll(sn);
				  if (status == MI_OK) {
					  RC_size = MFRC522_SelectTag(sn);
						  if(RC_size != 0){
							  status = MFRC522_Auth(PICC_AUTHENT1A, 2, sectorKeyA[2], sn); //authenticate card
							  if(status == MI_OK){
								  status = MFRC522_Read(2, str1); //read the 2 sector
								  if(status == MI_OK){
									  //sprintf((char *)buff, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n\r", str1[0],str1[1],str1[2],str1[3],str1[4],str1[5],str1[6],str1[7], str1[8],str1[9],str1[10],str1[11],str1[12],str1[13],str1[14],str1[15]);
									  sprintf((char *)buff, "CARD DATA: \"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\" ", str1[0],str1[1],str1[2],str1[3],str1[4],str1[5],str1[6],str1[7], str1[8],str1[9],str1[10],str1[11],str1[12],str1[13],str1[14],str1[15]);
									  HAL_UART_Transmit(&huart2, buff, sizeof buff/sizeof buff[0], 1000);
									  //HAL_UART_Transmit(&huart2, text1, sizeof text1/sizeof text1[0], 1000);
									  cardRead = 1;
								  }

								  if(status == MI_ERR){
									  led(1);
									  cardRead1 = 0;
									  break;

								  }
							  }
						  }
					  }

				}
			}
HAL_Delay(20);
MFRC522_Init();
	while(cardRead2 == 0)	{
		MFRC522_Init();
		status = MFRC522_Request(PICC_REQIDL, str);
		if (status == MI_OK) {
			status = MFRC522_Anticoll(sn);
			  if (status == MI_OK) {
				  RC_size = MFRC522_SelectTag(sn);
					  if(RC_size != 0){
						  status = MFRC522_Auth(PICC_AUTHENT1A, 2, sectorKeyA[2], sn);
						  if(status == MI_OK){
							  status = MFRC522_Write(2, p);
							  if(status == MI_OK){
								  //sprintf((char *)buff, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n\r", str1[0],str1[1],str1[2],str1[3],str1[4],str1[5],str1[6],str1[7], str1[8],str1[9],str1[10],str1[11],str1[12],str1[13],str1[14],str1[15]);
								  sprintf((char *)buff, "CARD WRITE: \"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\"\n\r", *(p), *(p + 1), *(p + 2), *(p + 3), *(p + 4), *(p + 5), *(p + 6), *(p + 7), *(p + 8), *(p + 9), *(p + 10), *(p + 11), *(p + 12), *(p + 13), *(p + 14), *(p + 15) );
								  //HAL_UART_Transmit(&huart2, buff, sizeof buff/sizeof buff[0], 1000);
								  HAL_UART_Transmit(&huart2, buff, sizeof buff/sizeof buff[0], 1000);
								  cardRead2 = 1;
							  }

							  if(status == MI_ERR){
								  led(1);
								  cardRead2 = 0;
								  break;

							  }
						  }
					  }
				  }

			}
		}
	cardRead = 0;
	cardRead1 = 0;
	cardRead2 = 0;
	HAL_Delay(1000);
	}

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
