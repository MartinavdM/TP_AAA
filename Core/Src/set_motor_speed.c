/*
 * set_motor_speed.c
 *
 *  Created on: Oct 18, 2023
 *      Author: peret
 */


#include "main.h"

/**
  * @brief  The function that calls PWM functions. "speed" is a percentage
  * @retval none
  */

void set_motor_speed(int speed) {
	uint32_t current_duty_cycle1 = 512; // The motor is initialized at a duty cylce of 50 % to start at null speed
	uint32_t current_duty_cycle2 = 511;
	uint32_t targeted_duty_cycle1 = speed*0.01*COUNTER_PERIOD;
	while(current_duty_cycle1 < targeted_duty_cycle1) {
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_duty_cycle1);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, current_duty_cycle2);
		current_duty_cycle1++;
		current_duty_cycle2 = COUNTER_PERIOD - current_duty_cycle1;
		HAL_Delay(10);
	}
	while(current_duty_cycle1 > targeted_duty_cycle1) {
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_duty_cycle1);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, current_duty_cycle2);
		current_duty_cycle1--;
		current_duty_cycle2 = COUNTER_PERIOD - current_duty_cycle1;
		HAL_Delay(10);
	}
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_duty_cycle1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, current_duty_cycle2);
}
