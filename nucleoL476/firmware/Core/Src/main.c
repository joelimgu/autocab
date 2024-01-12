/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>

#include "iks01a2.h"
#include "can.h"
#include "scheduler.h"
#include "globalvar.h"
#include <gnss_nmea_parser.h>
#include <gnss_ubx_parser.h>

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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

extern CAN_HandleTypeDef hcan;

int imuInit = 0;

uint8_t rxBufferGNSS; 	//Buffer to store data received from UART
char nmeaFrame[200];	//Frame received from UART
int nmeaFrameValid = 0;
int ubxFrameValid = 0;

struct UbxFrame rxUbxFrame;
struct UbxFrame ubxFrame;
int ubxSync1Received = 0;
int ubxReceivePayload = 0;
int processUbxFrame = 0;
int rx_index_ubx = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void CAN_FIFO_0_Callback(CAN_RxHeaderTypeDef *rxHeader, uint8_t data[]);
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
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	/* Initialization of IKS01A2 sensors */
	if (IKS01A2_Init()) {
		imuInit = 1; // IMU initialize correctly
	}

	/* Initialization of CAN */
	/* Todo: activate it when a driver is connected
	 * otherwise, it is flooding system with interrupt
	 */
	CAN_Init();
	CAN_AddRXCallback(CAN_FIFO_0_Callback);

	/* Various initializations */
	GLOBVAR_Init();
	SCHEDULER_Init();
	SCHEDULER_Run();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

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

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00702991;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Analog filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  HAL_UART_Receive_IT(&huart2, &rxBufferGNSS, 1);
  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart -> Instance == USART2)
    {
    	if (processUbxFrame){	//Decoding UBX frame

    		if (rx_index_ubx == 2) {
    			rxUbxFrame.class = rxBufferGNSS;
    			rx_index_ubx++;
    		}
    		else if (rx_index_ubx == 3){
    			rxUbxFrame.id = rxBufferGNSS;
    			rx_index_ubx++;
    		}
    		else if (rx_index_ubx == 4){

    		    rxUbxFrame.length = rxBufferGNSS;
    		    rx_index_ubx++;
    		}else if (rx_index_ubx == 5){

    		    rxUbxFrame.length = (rxBufferGNSS << 8) + rxUbxFrame.length;
    		    if (rxUbxFrame.length != UBX_PAYLOAD_LENGTH){
    		    	processUbxFrame = 0;
    		    }
    		    rx_index_ubx++;

    		}else if (rx_index_ubx>=6 && (rx_index_ubx < 6 + rxUbxFrame.length)){	//Payload
    			rxUbxFrame.payload[rx_index_ubx - 6] = rxBufferGNSS;
    			rx_index_ubx++;

    		}else if (rx_index_ubx == 6 + rxUbxFrame.length){
    			rxUbxFrame.checksumA = rxBufferGNSS;
    			rx_index_ubx++;

    		}else if (rx_index_ubx == 7 + rxUbxFrame.length){
    			rxUbxFrame.checksumB = rxBufferGNSS;
    			rx_index_ubx = 0;
    			processUbxFrame = 0;	//End of UBX frame

    			if (rxUbxFrame.class == 0x1 && rxUbxFrame.id==0x7){ //Check if UBX-NAV-PVT frame

    				//Testing checksum
    				uint8_t  checksumData[rxUbxFrame.length + 4];
    				uint8_t cka, ckb;

    				checksumData[0] = rxUbxFrame.class;
    				checksumData[1] = rxUbxFrame.id;
    				checksumData[2] = rxUbxFrame.length;
    				checksumData[3] = rxUbxFrame.length >> 8;

    				for (int i = 4; i<= 3 + rxUbxFrame.length; i++){
    					checksumData[i] = (uint8_t)rxUbxFrame.payload[i-4];
    				}

    				UBX_Fletcher_Checksum (checksumData,4 + rxUbxFrame.length, &cka, &ckb);
    				if (cka == rxUbxFrame.checksumA && ckb == rxUbxFrame.checksumB){

    					ubxFrame = rxUbxFrame;	//Copy "dynamic" frame to "static" frame
    					ubxFrameValid = 1;
    				}
    			}

    		}


    	}else {	//Decoding NMEA frame

			static char rx_buffer[LINEMAX];   // Local holding buffer to build line
			static int rx_index = 0;


			if ((rxBufferGNSS == '\r') || (rxBufferGNSS == '\n')) // Is this an end-of-line condition, either will suffice?
			{
			  if (rx_index != 0) // Line has some content
			  {
				memcpy((void *)nmeaFrame, rx_buffer, rx_index); // Copy to static line buffer from dynamic receive buffer
				nmeaFrame[rx_index] = 0; // Add terminating NUL
				nmeaFrameValid = 1; // flag new line valid for processing

				rx_index = 0; // Reset content pointer
			  }
			}
			else
			{
			  if ((rxBufferGNSS == '$') || (rx_index == LINEMAX)) // If resync or overflows pull back to start
				rx_index = 0;

			  rx_buffer[rx_index++] = rxBufferGNSS; // Copy to buffer and increment
			}
    	}

    	//Check if UBX frame is received
    	if (rxBufferGNSS == UBX_SYNC_CHAR2 && ubxSync1Received){

    		rxUbxFrame.syncChar1 = UBX_SYNC_CHAR1;
    		rxUbxFrame.syncChar2 = UBX_SYNC_CHAR2;
			processUbxFrame = 1;	//We are receiving a UBX frame
			rx_index_ubx=2;
			ubxSync1Received = 0;
		}

		if (rxBufferGNSS == UBX_SYNC_CHAR1){
			ubxSync1Received = 1;
		}

    	HAL_UART_Receive_IT(&huart2, &rxBufferGNSS, 1);
    }
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
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_8, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD2_Pin PA8 */
	GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB4 PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void CAN_FIFO_0_Callback(CAN_RxHeaderTypeDef *rxHeader, uint8_t data[])
{

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
