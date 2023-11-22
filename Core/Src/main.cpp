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
#ifdef __cplusplus
extern "C" {
#endif
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "math.h"
#include "aimAssist.hpp"
#include "reload.hpp"
#include "shooting.hpp"
#include "infoAndStatus.hpp"
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
TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*LED Relate BEGIN*/
const int Total_LEDs = 20;		//There are 30 leds, but we are only going to use the first 20 LEDs.
uint8_t LEDs_Data[Total_LEDs][3];
uint8_t LEDs_Data_Temp[Total_LEDs][3];	// For brightness
int dataSent_Finish = 0;				// Flag for DMA control

void Set_LED (int LEDidx, int Red, int Green, int Blue){	//0-255 scale
	LEDs_Data[LEDidx][0] = Green;
	LEDs_Data[LEDidx][1] = Red;
	LEDs_Data[LEDidx][2] = Blue;
}

void Set_Brightness (int brightness){		// 0-100
	//Make sure value is from 0 to 100
	brightness = (brightness < 0) ? 0 : (brightness > 100) ? 100 : brightness;

	//Tangent for linear scaling
	int brightness_45 = brightness*45/100;	// 0-45
	float angle = 90-brightness_45;  	// in degree

	for (int i=0; i<Total_LEDs; i++){
		for (int j=0; j<3; j++){
			LEDs_Data_Temp[i][j] = LEDs_Data[i][j] / tan(angle*M_PI / 180);
		}
	}

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	dataSent_Finish = 1;
}

void WS2812B_LED_Data_Send()
{
    uint32_t indx = 0;
    uint32_t colorData;			//use 32 bits as need transmit 24 bits RGB.
    uint16_t pwmData[24*Total_LEDs + 45];

    for (int i = 0; i < Total_LEDs ; i++){
    	//24 bit, G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
        colorData = (LEDs_Data_Temp[i][0] << 16) |
        				(LEDs_Data_Temp[i][1] << 8) |
									LEDs_Data_Temp[i][2];

    	//High bit is sent first, follow data sheet
        for (int j = 23; j >= 0; j--)
            pwmData[indx++] = (colorData & (1 << j)) ? 60 : 30;	// 1s: ~64% of 90, 0s: ~32% of 90
    }
    //Reset code which is above 50 us of 0s, period of clock is 1.25us
    for (int i = 0; i < 45; i++)
    	pwmData[indx++] = 0;

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);

    while (!dataSent_Finish){
    	// Wait for data to be sent
    }
    // Reset the flag
    dataSent_Finish = 0;
}
/*LED Related END*/

extern WaterGun::currentInfoDisplay infoDisplay;
extern reloadingProcess::Reload Reloadobj;

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
  MX_FSMC_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  LCD_INIT();

  DEBUG_DELAY();
  LCD_Write_Cmd(0x36);
  LCD_Write_Cmd(0xC8);
  DEBUG_DELAY();
  DEBUG_DELAY();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  const char* Name = "Gupta";
	  LCD_DrawString(100,100,Name);

//	  infoDisplay.changeStatus(WaterGun::STATUS::OFF_STATE);	//test, status should be change be interrupt using K1
	  infoDisplay.changeStatus(WaterGun::STATUS::RELOAD_STATE);	//test, status should be change be interrupt using K1
//	  infoDisplay.changeStatus(WaterGun::STATUS::SINGLE_SHOOT_STATE);	//test, status should be change be interrupt using K1
//	  infoDisplay.changeStatus(WaterGun::STATUS::CONTINIOUS_SHOOT_STATE);	//test, status should be change be interrupt using K1

	  WaterGun::STATUS curStatus = infoDisplay.getStatus();
	  if (curStatus == WaterGun::STATUS::OFF_STATE){
		 //LED off state motion (Breathing)
		  for (int i=0; i<20; i++)
		  {
			  Set_LED(i, 255-2*i, 255-2*i, 4*i);	//Yellow to less yellow
		  }
		  for (int i=0; i<20; i++)
		  {
			  Set_Brightness(3*i);
			  WS2812B_LED_Data_Send();
			  HAL_Delay (25);
		  }
		  for (int i=19; i>=0; i--)
		  {
			  Set_Brightness(3*i);
			  WS2812B_LED_Data_Send();
			  HAL_Delay (25);
		  }
	  }
	  else if (curStatus == WaterGun::STATUS::RELOAD_STATE){
		  //If switch is pressed
		  if (Reloadobj.getTriggerState() == true){
			  //LED reload motion
			  for (int i=0; i<10+3; i++)
			  {
				  if (i<10){
					  //Turn on head of middleleft LED
					  Set_LED(9-i, 42, 254, 183);			//Cyan
					  //Turn on head of middleright LED
					  Set_LED(10+i, 42, 254, 183);			//Cyan
				  }
				  if ( (i-3) >= 0 ){
					  //Turn on tail of middleleft LED
					  Set_LED(9-(i-3), 0, 0, 0);
					  //Turn off tail of middleright LED
					  Set_LED(10+(i-3), 0, 0, 0);
				  }
				  Set_Brightness(60);
				  WS2812B_LED_Data_Send();
				  HAL_Delay (77);
			  }
		  }
		  //If switch not yet pressed
		  else{
			  //LED reload motion (Breathing)
			  for (int i=0; i<20; i++)
			  {
				  Set_LED(i, 42, 254, 183);					//Cyan
			  }
			  for (int i=0; i<20; i++)
			  {
				  Set_Brightness(3*i);
				  WS2812B_LED_Data_Send();
				  HAL_Delay (25);
			  }
			  for (int i=19; i>=0; i--)
			  {
				  Set_Brightness(3*i);
				  WS2812B_LED_Data_Send();
				  HAL_Delay (25);
			  }
		  }
	  }
	  else if (curStatus == WaterGun::STATUS::SINGLE_SHOOT_STATE){
		  //LED shoot out motion
		  for (int i=0; i<10+3; i++)
		  {
			  if (i<10){
				  //Turn on head of leftmost LED
				  Set_LED(i, 138, 43, 226);			//Blue-purple
				  //Turn on head of rightmost LED
				  Set_LED(19-i, 138, 43, 226);		//Blue-purple
			  }
			  if ( (i-3) >= 0 ){
				  //Turn on tail of leftmost LED
				  Set_LED(i-3, 0, 0, 0);
				  //Turn off tail of rightmost LED
				  Set_LED(19-i+3, 0, 0, 0);
			  }
			  Set_Brightness(70);
			  WS2812B_LED_Data_Send();
			  HAL_Delay (100);
		  }
	  }
	  else{	//CONTINIOUS_SHOOT_STATE
		  /*Add here*/
	  }
	  GPIO_PinState pinUpperBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
	  GPIO_PinState pinLowerBottle = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
	  LCD_DrawString(50,200,"                ");
	  if (pinUpperBottle == GPIO_PIN_RESET && pinLowerBottle == GPIO_PIN_RESET){
		  LCD_DrawString(50,200,"Water Full");
	  }
	  else if(pinUpperBottle == GPIO_PIN_SET && pinLowerBottle == GPIO_PIN_RESET){
		  LCD_DrawString(50,200,"Have some water");
	  }
	  else if(pinUpperBottle == GPIO_PIN_SET && pinLowerBottle == GPIO_PIN_SET){
		  LCD_DrawString(50,200,"Not enough water");
	  }
	  else
		  LCD_DrawString(50,200,"Impossible");

//	  LCD_DrawEllipse(120,160,75,25,BLACK);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 89;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
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

#ifdef __cplusplus
}
#endif
