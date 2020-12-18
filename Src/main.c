/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "gpio.h"
#include "tim.h"
#include "display.h"
#include "i2c.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "lps25hb.h"
#include "hts221.h"
#include <stdio.h>
#include <math.h>

void SystemClock_Config(void);

extern uint64_t disp_time;

char string[16];
char string_disp[4];
uint8_t pos = 0;
uint8_t backwards = 0;
uint8_t switch_state = 1;
uint8_t change_state = 1;
float mag[3], acc[3], azimuth, alt;
float* press, hum, temp;

float get_altitude(float p, int16_t temp);
float get_azimuth(float x, float y);

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

  setSegments();
  setDigits();

  LL_mDelay(2000);

  resetDigits();
  resetSegments();

  while(!lsm6ds0_init());
  while(!lps25hb_init());
//  while(!hts221_init());
  while(!lis3mdl_init());

  MX_TIM3_Init();

  while (1)
  {
	  if (change_state) {
		  pos = 0; change_state = 0;
	  }

	  /* Display chosen state */
	  if (switch_state == 1) {
		  lis3mdl_get_mag(mag, (mag+1), (mag+2));
		  azimuth = get_azimuth(*mag, *(mag+1));
		  snprintf(string, sizeof(string), "MAG_%2.1f", azimuth);
	  }

	  else if (switch_state == 2) {
//		  temp = hts221_get_temp();
		  snprintf(string, sizeof(string), "tEMP_");;
	  }

	  else if (switch_state == 3) {
//		  hts221_get_hum(hum);
		  snprintf(string, sizeof(string), "HUM_");
	  }

	  else if (switch_state == 4) {
//		  lps25hb_get_press(press);
		  snprintf(string, sizeof(string), "bar_");
	  }

	  else if (switch_state == 5) {
//		  lps25hb_get_press(press);
//		  int16_t temperature = lps25hb_get_temp();
//		  alt = get_altitude(*press, temperature);
		  snprintf(string, sizeof(string), "ALt_");
	  }

	  else {
		snprintf(string, sizeof(string), "Error");
	  }

	  /* Get direction of displaying values */
	  if(backwards) {
		  for (uint8_t i = 0; i < 4; i++) {
			  string_disp[i] = string[pos-2+i];
		  }

		  --pos;

		  if (pos <= 1) {
			  backwards = 0;
		  }
	  }

	  else {
		  for (uint8_t i = 0; i < 4; i++) {
			  string_disp[i] = string[pos+i];
		  }

		  ++pos;

		  if (pos >= (sizeof(string)/sizeof(string[1])-4)) {
			  backwards = 1;
		  }
	  }

	  displayNumber(string_disp);
	  LL_mDelay(500);
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SYSTICK_EnableIT();
  LL_SetSystemCoreClock(8000000);
}

float get_azimuth(float x, float y)
{
	return atan2(x, y) * 180.0/M_PI;
}

float get_altitude(float p, int16_t temp)
{
	float p0 = 1013.25;
	double press = p0/p;
	double exponent = 1/5.257;
	float numerator = (pow(press, exponent) - 1) * temp;
	return numerator/0.0065;
}

void set_state(uint8_t state)
{
	switch_state = state;
	change_state = 1;
}

uint8_t get_state()
{
	return switch_state;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
