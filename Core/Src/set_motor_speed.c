/**
 * \file set_motor_speed.c
 * \brief Cette fonction permet de générer les signaux de PWMs pilotant notre moteur. Elle prend en paramètre "speed" : un pourcentage entier correspondant à la vitesse du moteur.
 * \author Paul-Etienne Rétaux
 * \date 30 octobre 2023
 *
 *
 */

#include "main.h"

/**
 * @brief  The function that calls PWM functions. "speed" is a percentage
 * @retval none
 */

uint32_t current_duty_cycle1 = 511; // The motor is initialized at a duty cylce of 50 % to start at null speed
uint32_t current_duty_cycle2 = 512;
extern uint8_t wait_flag;

void set_motor_speed(int speed) {
	uint32_t targeted_duty_cycle1 = speed * 0.01 * COUNTER_PERIOD;
	while (current_duty_cycle1 < targeted_duty_cycle1) {
		if (wait_flag) {
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_duty_cycle1);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, current_duty_cycle2);
			current_duty_cycle1++;
			current_duty_cycle2 = COUNTER_PERIOD - current_duty_cycle1;
			wait_flag = 0;
		}
	}
	while (current_duty_cycle1 > targeted_duty_cycle1) {
		if (wait_flag) {
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_duty_cycle1);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, current_duty_cycle2);
			current_duty_cycle1--;
			current_duty_cycle2 = COUNTER_PERIOD - current_duty_cycle1;
			wait_flag = 0;
		}
	}
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, current_duty_cycle1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, current_duty_cycle2);
}


