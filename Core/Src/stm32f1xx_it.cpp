/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "infoAndStatus.hpp"
#include "reload.hpp"
#include "shooting.hpp"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

WaterGun::currentInfoDisplay infoDisplay;
reloadingProcess::Reload Reloadobj(400,0);
shootingProcess::singleShot singleShotobj(0, 1000, 500, 1000);
shootingProcess::continousShots continousShotsobj(0, 700, 500, 1000);

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_tim1_ch1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

///**
//  * @brief This function handles EXTI line0 interrupt.
//  */
//void EXTI0_IRQHandler(void)
//{
//  /* USER CODE BEGIN EXTI0_IRQn 0 */
//
//	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
//	{
//		/*Key1 code BEGIN*/
//		//Check current gun mode
//		WaterGun::STATUS curStatus = infoDisplay.status;
//		if (curStatus == WaterGun::STATUS::OFF_STATE){
//			infoDisplay.status = WaterGun::STATUS::RELOAD_STATE;	//Status changes by interrupt using K1
//		}
//		else if (curStatus == WaterGun::STATUS::RELOAD_STATE){
//			infoDisplay.status = WaterGun::STATUS::SINGLE_SHOOT_STATE;
//		  }
//		else if (curStatus == WaterGun::STATUS::SINGLE_SHOOT_STATE){
//			infoDisplay.status = WaterGun::STATUS::CONTINIOUS_SHOOT_STATE;
//		}
//		else{								//CONTINIOUS_SHOOT_STATE
//			infoDisplay.status = WaterGun::STATUS::OFF_STATE;
//		}
//		/*Key1 code END*/
//		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
//		HAL_GPIO_EXTI_Callback(GPIO_PIN_0);
//	}
//
//  /* USER CODE END EXTI0_IRQn 0 */
// // HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
//  /* USER CODE BEGIN EXTI0_IRQn 1 */
//
//  /* USER CODE END EXTI0_IRQn 1 */
//}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_tim1_ch1);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
	/* USER CODE BEGIN EXTI9_5_IRQn 0 */
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
	{
		/*Switch code BEGIN*/

		//Switch is pressed action
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) ==  GPIO_PIN_RESET){

			//Check current gun mode
			WaterGun::STATUS curStatus = infoDisplay.status;
			if (curStatus == WaterGun::STATUS::OFF_STATE){
				//Do nothing
			}
			else if (curStatus == WaterGun::STATUS::RELOAD_STATE){
				Reloadobj.setTriggerState(true);
			  }
			else if (curStatus == WaterGun::STATUS::SINGLE_SHOOT_STATE){
				/*Add here*/
				singleShotobj.setTriggerState(true);
			}
			else{								//CONTINIOUS_SHOOT_STATE
				/*Add here*/
				continousShotsobj.setTriggerState(true);
			}

			//Print
			LCD_Clear(222,84-10*8,16,10*8,0xffff);
			LCD_DrawString(222,84,"Switch on");
		}
		//Switch is released action
		else{
			//Check current gun mode
			WaterGun::STATUS curStatus = infoDisplay.status;
			if (curStatus == WaterGun::STATUS::OFF_STATE){
				//Do nothing
			}
			else if (curStatus == WaterGun::STATUS::RELOAD_STATE){
				Reloadobj.setTriggerState(false);
			  }
			else if (curStatus == WaterGun::STATUS::SINGLE_SHOOT_STATE){
				/*Add here*/
				singleShotobj.setTriggerState(false);
			}
			else{								//CONTINIOUS_SHOOT_STATE
				/*Add here*/
				continousShotsobj.setTriggerState(false);
			}

			//Print
			LCD_Clear(222,84-10*8,16,10*8,0xffff);
			LCD_DrawString(222,84,"Switch off");
		}
		/*Switch code END*/
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_8);
	}
	/* USER CODE END EXTI9_5_IRQn 0 */
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

//void EXTI15_10_IRQHandler(void)
//{
//  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
//
//  /* USER CODE END EXTI15_10_IRQn 0 */
//    int c = 'o';
//  /* USER CODE BEGIN EXTI15_10_IRQn 1 */
//
//  /* USER CODE END EXTI15_10_IRQn 1 */
//}

/* USER CODE BEGIN 1 */
#ifdef __cplusplus
}
#endif

/* USER CODE END 1 */
