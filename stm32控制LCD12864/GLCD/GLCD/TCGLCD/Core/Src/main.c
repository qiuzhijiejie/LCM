/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void BusyL(void);
void BusyR(void);
void Locatexy(void);
void WriteCommandL(uint8_t CommandByte);
void WriteCommandR(uint8_t CommandByte);
uint8_t ReadData(void);
void WriteData(uint8_t DataByte);
void LcmClear(void);
void LcmInit(void);
void LcmPutDots(uint8_t DotByte, uint8_t DotByte1);
//void LcmPutBMP( unsigned char *puts );
void LcmPutBMP(const uint8_t *puts);
void LcmReverseBMPP(const uint8_t *puts);
void Delay(uint32_t ms);
void Display_Chinese(void);
void Display_16_16(const uint8_t *F, uint8_t i);
void Display_8_8(const uint8_t *C, uint8_t i);
void Display_character(void);
void EXTI0_1_IRQHandler(void);
void writePortSet(void);
uint8_t ReadDataFromPins(void);
void Printdata(unsigned char data);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t Page;         // Current page variable
uint8_t Col;          // Current column variable
uint8_t character[]= {/* Character data */};
//const uint8_t font[] = {/* Font data */};
//unsigned char font[];
//unsigned char font[] = {/* Font data */};
const uint8_t BMP3[] = {/* Bitmap data */};
const uint8_t BMP4[] = {/* Bitmap data */};

unsigned char font[] = {
    0x3f, 0x15, 0x15, 0x15, 0x15, 0x3f, 0x00, 0x00,  // æœ?
    0x00, 0x00, 0x48, 0x54, 0xfe, 0x54, 0x24, 0x00,  // $
    0x00, 0x00, 0x3e, 0x41, 0x79, 0x49, 0x32, 0x00   // G
};

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

IWDG_HandleTypeDef hiwdg;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_IWDG_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	LcmInit();
	LcmClear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		LcmPutDots(0x55, 0xaa);
		HAL_Delay(600);
		LcmPutDots(0xaa, 0x55);
		HAL_Delay(600);

		LcmPutDots(0x55, 0x55);
		HAL_Delay(600);
		LcmPutDots(0xaa, 0xaa);
		HAL_Delay(600);

		LcmPutDots(0xff, 0x00);
		HAL_Delay(600);
		LcmPutDots(0x00, 0xff);
		HAL_Delay(600);

		LcmPutBMP(BMP3);
		HAL_Delay(1000);

		LcmReverseBMPP(BMP3);
		HAL_Delay(1000);
		LcmPutBMP(BMP4);
		HAL_Delay(1000);
		LcmReverseBMPP(BMP4);
		HAL_Delay(1000);
		LcmClear();
		Display_Chinese();
		HAL_Delay(1000);
		Display_character();

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LEDK_GPIO_O_P_Pin|LCD_CS2_GPIO_O_P_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_E_GPIO_O_P_Pin|LCD_RS_GPIO_O_P_Pin|LCD_CS1_GPIO_O_P_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_R_W_GPIO_O_P_Pin|DB0_GPIO_O_P_Pin|DB1_GPIO_O_P_Pin|DB2_GPIO_O_P_Pin
                          |DB3_GPIO_O_P_Pin|DB4_GPIO_O_P_Pin|DB5_GPIO_O_P_Pin|DB6_GPIO_O_P_Pin
                          |DB7_GPIO_O_P_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_O_P_GPIO_Port, LCD_RST_GPIO_O_P_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISP_INT_GPIO_Port, DISP_INT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LEDK_GPIO_O_P_Pin LCD_CS2_GPIO_O_P_Pin */
  GPIO_InitStruct.Pin = LEDK_GPIO_O_P_Pin|LCD_CS2_GPIO_O_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_E_GPIO_O_P_Pin LCD_RS_GPIO_O_P_Pin */
  GPIO_InitStruct.Pin = LCD_E_GPIO_O_P_Pin|LCD_RS_GPIO_O_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_R_W_GPIO_O_P_Pin LCD_RST_GPIO_O_P_Pin DB0_GPIO_O_P_Pin DB1_GPIO_O_P_Pin
                           DB2_GPIO_O_P_Pin DB3_GPIO_O_P_Pin DB4_GPIO_O_P_Pin DB5_GPIO_O_P_Pin
                           DB6_GPIO_O_P_Pin DB7_GPIO_O_P_Pin */
  GPIO_InitStruct.Pin = LCD_R_W_GPIO_O_P_Pin|LCD_RST_GPIO_O_P_Pin|DB0_GPIO_O_P_Pin|DB1_GPIO_O_P_Pin
                          |DB2_GPIO_O_P_Pin|DB3_GPIO_O_P_Pin|DB4_GPIO_O_P_Pin|DB5_GPIO_O_P_Pin
                          |DB6_GPIO_O_P_Pin|DB7_GPIO_O_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DISP_INT_Pin */
  GPIO_InitStruct.Pin = DISP_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DISP_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_EN_Pin SW4_GPIO_I_P_Pin SW3_GPIO_I_P_Pin SW2_GPIO_I_P_Pin
                           SW1__GPIO_I_P_Pin */
  GPIO_InitStruct.Pin = DISP_EN_Pin|SW4_GPIO_I_P_Pin|SW3_GPIO_I_P_Pin|SW2_GPIO_I_P_Pin
                          |SW1__GPIO_I_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_CS1_GPIO_O_P_Pin */
  GPIO_InitStruct.Pin = LCD_CS1_GPIO_O_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_CS1_GPIO_O_P_GPIO_Port, &GPIO_InitStruct);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB9);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PC14);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PA9);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PA10);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void BusyL(void) {
	uint8_t flag = 0xff;
	HAL_GPIO_WritePin(LCD_CS1_GPIO_O_P_GPIO_Port, LCD_CS1_GPIO_O_P_Pin, GPIO_PIN_SET);  // CS1 = 1
	HAL_GPIO_WritePin(LCD_CS2_GPIO_O_P_GPIO_Port, LCD_CS2_GPIO_O_P_Pin, GPIO_PIN_RESET); // CS2 = 0
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_SET);   // RW = 1
	writePortSet();
	while ((flag & 0x80) == 0x80) {
		HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET); // E = 1
		HAL_Delay(1);
		flag= ReadDataFromPins();
		HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	}
}

void BusyR(void) {
	uint8_t flag = 0xff;
	HAL_GPIO_WritePin(LCD_CS1_GPIO_O_P_GPIO_Port, LCD_CS1_GPIO_O_P_Pin, GPIO_PIN_RESET); // CS1 = 0
	HAL_GPIO_WritePin(LCD_CS2_GPIO_O_P_GPIO_Port, LCD_CS2_GPIO_O_P_Pin, GPIO_PIN_SET);   // CS2 = 1
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // E = 0

	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_SET);   // RW = 1
	writePortSet();
	while ((flag & 0x80) == 0x80) {
		HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET); // E = 1
		HAL_Delay(1);
		flag= ReadDataFromPins();
		flag = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6); // Read from the status pin
		HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	}
}

void Locatexy(void) {
	unsigned char x, y;
	uint8_t Col = 0x00;  // Replace with actual value or argument
	uint8_t Page = 0x00; // Replace with actual value or argument

	switch (Col & 0xc0) {
	case 0:
		BusyL();
		break;
	case 0x40:
		BusyR();
		break;
	}

	x = (Col & 0x3F) | 0x40; // col.and.0x3f.or.Set X Address
	y = (Page & 0x07) | 0xB8; // row.and.0x07.or.set Page

	// Set RS and RW to 0
	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_RESET); // RW = 0

	// Send Y Address
	Printdata(y);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET);  // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	HAL_Delay(1);

	// Send X Address
	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_RESET); // RW = 0
	Printdata(x);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET);  // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	HAL_Delay(1);
}

void WriteCommandL(uint8_t CommandByte) {
	BusyL();
	Printdata(CommandByte);
	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET);  // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	HAL_Delay(1);
}

void WriteCommandR(uint8_t CommandByte) {
	BusyR();
	Printdata(CommandByte);
	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_RESET); // RS = 0
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_RESET); // RW = 0
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET);  // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	HAL_Delay(1);
}

unsigned char ReadData(void) {
	unsigned char DataByte = 0;
	Locatexy();
	// Set RS and RW
	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_SET);

	writePortSet();
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET);  // E = 1
	HAL_Delay(1);

	// Read data
	DataByte = (uint8_t)HAL_GPIO_ReadPin(GPIOA, DB0_GPIO_O_P_Pin) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB1_GPIO_O_P_Pin) << 1) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB2_GPIO_O_P_Pin) << 2) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB3_GPIO_O_P_Pin) << 3) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB4_GPIO_O_P_Pin) << 4) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB5_GPIO_O_P_Pin) << 5) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB6_GPIO_O_P_Pin) << 6) |
			(uint8_t)(HAL_GPIO_ReadPin(GPIOA, DB7_GPIO_O_P_Pin) << 7);

	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_CS1_GPIO_O_P_GPIO_Port, LCD_CS1_GPIO_O_P_Pin, GPIO_PIN_RESET); // CS1 = 0
	HAL_GPIO_WritePin(LCD_CS2_GPIO_O_P_GPIO_Port, LCD_CS2_GPIO_O_P_Pin, GPIO_PIN_RESET);   // CS2 = 0

	return DataByte;
}

void WriteData(unsigned char DataByte) {
	Locatexy();
	HAL_GPIO_WritePin(LCD_RS_GPIO_O_P_GPIO_Port, LCD_RS_GPIO_O_P_Pin, GPIO_PIN_SET); // RS = 1
	HAL_GPIO_WritePin(LCD_R_W_GPIO_O_P_GPIO_Port, LCD_R_W_GPIO_O_P_Pin, GPIO_PIN_RESET); // RW = 0

	// Define an array of GPIO pins corresponding to DB0 to DB7
	GPIO_TypeDef* dataPorts[8] = {
			GPIOA, GPIOA, GPIOA, GPIOA, // Adjust this array to match your actual GPIO port setup
			GPIOA, GPIOA, GPIOA, GPIOA
	};
	uint16_t dataPins[8] = {
			DB0_GPIO_O_P_Pin, DB1_GPIO_O_P_Pin, DB2_GPIO_O_P_Pin, DB3_GPIO_O_P_Pin,
			DB4_GPIO_O_P_Pin, DB5_GPIO_O_P_Pin, DB6_GPIO_O_P_Pin, DB7_GPIO_O_P_Pin
	};

	// Write each bit of DataByte to the corresponding data pin
	for (int i = 0; i < 8; i++) {
		GPIO_PinState pinState = (DataByte & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		HAL_GPIO_WritePin(dataPorts[i], dataPins[i], pinState);
	}

	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_SET);  // E = 1
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_GPIO_O_P_GPIO_Port, LCD_E_GPIO_O_P_Pin, GPIO_PIN_RESET); // E = 0
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_CS1_GPIO_O_P_GPIO_Port, LCD_CS1_GPIO_O_P_Pin, GPIO_PIN_RESET); // CS1 = 0
	HAL_GPIO_WritePin(LCD_CS2_GPIO_O_P_GPIO_Port, LCD_CS2_GPIO_O_P_Pin, GPIO_PIN_RESET);   // CS2 = 0
}

void LcmClear(void) {
	for (Page = 0; Page < 8; Page++) {
		for (Col = 0; Col < 128; Col++) {
			WriteData(0);
		}
	}
}

void LcmInit( void )
{

	WriteCommandL(0x3f); //DISPALY ON
	WriteCommandR(0x3f);

	WriteCommandL(0xc0); //SET START ADDRESS
	WriteCommandR(0xc0);

	WriteCommandL(0x3f); //DISPLAY ON
	WriteCommandR(0x3f);

}

void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 )
{
	Page = 0;
	Col  = 0;
	for(Page=0;Page<8;Page++)
	{
		for(Col=0;Col<128;Col++)
		{
			WriteData( DotByte );
			Col++;
			WriteData( DotByte1 );
		}

	}
}

void LcmPutBMP(const uint8_t *puts)
{
	unsigned int X=0;
	Page = 0;
	Col  = 0;
	for(Page=0;Page<8;Page++)
	{
		for(Col=0;Col<128;Col++)
		{
			WriteData( puts[X] );
			X++;
		}
	}
}

void LcmReverseBMPP(const uint8_t *puts)
{
	unsigned int X=0;
	Page = 0;
	Col  = 0;
	for(Page=0;Page<8;Page++)
	{
		for(Col=0;Col<128;Col++)
		{
			WriteData(~puts[X] );
			X++;
		}
	}
}

//void Display_8_8(const uint8_t C[], uint8_t i)
//{
//    uint8_t k;
//    uint8_t num = i * 8;  // Calculate the starting index based on the mode
//
//    for (k = 0; k < 8; k++)
//    {
//        WriteData(C[num + k]);  // Write data to the display
//        Col += 1;  // Move to the next column
//    }
//}

void Display_8_8(const uint8_t *C, uint8_t i)
{
    uint8_t k;
    uint8_t num = i * 8;  // Calculate the starting index based on the character index

    for (k = 0; k < 8; k++)
    {
        WriteData(C[num + k]);  // Write data to the display
        Col += 1;  // Move to the next column
    }
}
//void Display_16_16(const uint8_t F[], uint8_t i)
//{
//    uint8_t k;
//    uint8_t num = i * 32;  // Calculate the starting index based on the mode
//
//    // Display the first 16 rows
//    for (k = 0; k < 16; k++)
//    {
//        WriteData(F[num + k]);  // Write data to the display
//        Col += 1;  // Move to the next column
//    }
//    Col -= 16;  // Move back to the beginning of the next page
//    Page += 1;  // Move to the next page
//
//    // Display the next 16 rows
//    for (k = 16; k < 32; k++)
//    {
//        WriteData(F[num + k]);  // Write data to the display
//        Col += 1;  // Move to the next column
//    }
//    Page -= 1;  // Move back to the original page
//
//    HAL_Delay(80);  // Delay for visual effect
//}

void Display_16_16(const uint8_t *F, uint8_t i)
{
    uint8_t k;
    uint8_t num = i * 32;  // Calculate the starting index based on the character index

    // Display the first 16 rows
    for (k = 0; k < 16; k++)
    {
        WriteData(F[num + k]);  // Write data to the display
        Col += 1;  // Move to the next column
    }
    Col -= 16;  // Move back to the beginning of the next page
    Page += 1;  // Move to the next page

    // Display the next 16 rows
    for (k = 16; k < 32; k++)
    {
        WriteData(F[num + k]);  // Write data to the display
        Col += 1;  // Move to the next column
    }
    Page -= 1;  // Move back to the original page

    HAL_Delay(80);  // Delay for visual effect
}

void Display_character()
{
    unsigned char k, c;

    Col = 0;  // Initialize the column position

    // Display character in mode 0
    for (c = 0; c < 8; c++)
    {
        Page = c;  // Set the page
        for (k = 0; k < 16; k++)
        {
            Display_8_8(character, 0);  // Display character in mode 0
        }
        Col = 0;  // Reset column position
    }
    HAL_Delay(1500);  // Delay before switching modes

    // Display character in mode 1
    for (c = 0; c < 8; c++)
    {
        Page = c;  // Set the page
        for (k = 0; k < 16; k++)
        {
            Display_8_8(character, 1);  // Display character in mode 1
        }
        Col = 0;  // Reset column position
    }
    HAL_Delay(1500);  // Delay before switching modes

    // Display character in mode 2
    for (c = 0; c < 8; c++)
    {
        Page = c;  // Set the page
        for (k = 0; k < 16; k++)
        {
            Display_8_8(character, 2);  // Display character in mode 2
        }
        Col = 0;  // Reset column position
    }
    HAL_Delay(1000);  // Delay before ending the function
}
//void Display_Chinese()
//{
//    unsigned char a;
//
//    // Display the first set of 16x16 Chinese characters
//    Page = 0;
//    Col = 0;
//    for (a = 0; a < 8; a++)
//    {
//        Display_16_16(font, a);  // Display 16x16 font data
//    }
//
//    // Display the next set of 16x16 Chinese characters
//    Page = 2;
//    Col = 0;
//    for (a = 8; a < 16; a++)
//    {
//        Display_16_16(font, a);  // Display 16x16 font data
//    }
//
//    // Display the next set of 16x16 Chinese characters
//    Page = 4;
//    Col = 0;
//    for (a = 16; a < 24; a++)
//    {
//        Display_16_16(font, a);  // Display 16x16 font data
//    }
//
//    // Display the last set of 16x16 Chinese characters
//    Page = 6;
//    Col = 0;
//    for (a = 24; a < 32; a++)
//    {
//        Display_16_16(font, a);  // Display 16x16 font data
//    }
//}

void Display_Chinese()
{
    uint8_t a;

    // Display the first set of 16x16 Chinese characters
    Page = 0;
    Col = 0;
    for (a = 0; a < 8; a++)
    {
        Display_16_16(font, a);  // Display 16x16 font data
    }

    // Display the next set of 16x16 Chinese characters
    Page = 2;
    Col = 0;
    for (a = 8; a < 16; a++)
    {
        Display_16_16(font, a);  // Display 16x16 font data
    }

    // Display the next set of 16x16 Chinese characters
    Page = 4;
    Col = 0;
    for (a = 16; a < 24; a++)
    {
        Display_16_16(font, a);  // Display 16x16 font data
    }

    // Display the last set of 16x16 Chinese characters
    Page = 6;
    Col = 0;
    for (a = 24; a < 32; a++)
    {
        Display_16_16(font, a);  // Display 16x16 font data
    }
}

void writePortSet(void)

{
	//  uint8_t RDFP_temp = 0;
	// Set each pin high individually

	HAL_GPIO_WritePin(GPIOA, DB0_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB1_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB2_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB3_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB4_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB5_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB6_GPIO_O_P_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, DB7_GPIO_O_P_Pin, GPIO_PIN_SET);
}

uint8_t ReadDataFromPins(void)
{
	uint8_t RDFP_temp = 0;

	// Read each pin and combine the result
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB0_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 0) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB1_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 1) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB2_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 2) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB3_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 3) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB4_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 4) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB5_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 5) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB6_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 6) : 0;
	RDFP_temp |= (HAL_GPIO_ReadPin(GPIOA, DB7_GPIO_O_P_Pin) == GPIO_PIN_SET) ? (1 << 7) : 0;

	return RDFP_temp;
}

void Printdata(unsigned char data)
{

	if((data&0x01)==0x01)
	{
		HAL_GPIO_WritePin(GPIOA, DB0_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, DB0_GPIO_O_P_Pin, GPIO_PIN_RESET);
	}

	if((data&0x02)==0x02)
	{
		HAL_GPIO_WritePin(GPIOA, DB1_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, DB1_GPIO_O_P_Pin, GPIO_PIN_RESET);
	}

	if((data&0x04)==0x04)
	{
		HAL_GPIO_WritePin(GPIOA, DB2_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{HAL_GPIO_WritePin(GPIOA, DB2_GPIO_O_P_Pin, GPIO_PIN_RESET);}

	if((data&0x08)==0x08)
	{
		HAL_GPIO_WritePin(GPIOA, DB3_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{HAL_GPIO_WritePin(GPIOA, DB3_GPIO_O_P_Pin, GPIO_PIN_RESET);}

	if((data&0x10)==0x10)
	{
		HAL_GPIO_WritePin(GPIOA, DB4_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{HAL_GPIO_WritePin(GPIOA, DB4_GPIO_O_P_Pin, GPIO_PIN_RESET);}

	if((data&0x20)==0x20)
	{
		HAL_GPIO_WritePin(GPIOA, DB5_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{HAL_GPIO_WritePin(GPIOA, DB5_GPIO_O_P_Pin, GPIO_PIN_RESET);}

	if((data&0x40)==0x40)
	{
		HAL_GPIO_WritePin(GPIOA, DB6_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{HAL_GPIO_WritePin(GPIOA, DB6_GPIO_O_P_Pin, GPIO_PIN_RESET);}

	if((data&0x80)==0x80)
	{
		HAL_GPIO_WritePin(GPIOA, DB7_GPIO_O_P_Pin, GPIO_PIN_SET);
	}
	else{HAL_GPIO_WritePin(GPIOA, DB7_GPIO_O_P_Pin, GPIO_PIN_RESET);}


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
