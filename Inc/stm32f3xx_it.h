/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F3xx_IT_H
#define __STM32F3xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
 // defines for input port used by button
 #define		GPIO_PORT_BUTTON				GPIOB
 #define		GPIO_PIN_BUTTON					3

 // defines for "checkButtonState"
 #define		TRIGGER_RISE					0
 #define		TRIGGER_FALL					1

 #define		BUTTON_EXTI_TRIGGER				TRIGGER_FALL
 #define		BUTTON_EXTI_SAMPLES_WINDOW		30
 #define		BUTTON_EXTI_SAMPLES_REQUIRED	20

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI3_IRQHandler(void);
void TIM3_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
/* USER CODE BEGIN EFP */

uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t edge, uint8_t samples_window, uint8_t samples_required);

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F3xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
