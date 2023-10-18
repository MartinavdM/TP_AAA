/*
 * set_motor_speed.c
 *
 *  Created on: Oct 18, 2023
 *      Author: peret
 */


#include <stdio.h>
#include <stdlib.h>
#include "set_motor_speed.h"

/**
  * @brief  The function that calls PWM functions.
  * @retval none
  */

void set_motor_motor_speed(uint32_t speed) {
	targeted_duty_cycle1 = speed*0.01*COUNTER_PERIOD;
	targeted_duty_cycle2 = (COUNTER_PERIOD-targeted_duty_cycle1);
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
