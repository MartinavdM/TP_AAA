/**
 * \file speed_enslavement.c
 * \brief Ce fichier contient une fonction qui permet de calculer un courant de consigne grâce à un correcteur PI et prenant en compte des échantillons à un instant n et à un instant n-1 (préfixe "old_").
 * \author Paul-Etienne Rétaux
 * \date 30 octobre 2023
 *
 *
 */

#include "main.h"

extern float filtered_measured_speed;
extern int order_speed; // order_speed is a duty cycle; it will be converted in a speed in rad/s in the below function

float old_speed_error = 0.0;
float speed_error = 0.0;

float old_order_current = 0.0;
float order_current = 0.0;

void speed_enslavement(void) {
	float b_zero = 0.4775; // k_p+k_I*T_e/2 = 0.354+2.47*0.1/2
	float b_one = -0.2305; // k_I*T_e/2 - k_p = 2.47*0.1/2 - 0.354
	float physical_filtered_measured_speed = 2 * M_PI * filtered_measured_speed
			/ 60;
	float physical_order_speed = 2 * M_PI * (order_speed / 100 * 3000) / 60; // a ratio of the maximum rotation speed (3000 rpm) "physical_order_speed" is in rad/s
	speed_error = physical_filtered_measured_speed - physical_order_speed;
	order_current = old_order_current + b_zero * speed_error
			+ b_one * old_speed_error;
	old_order_current = order_current;
	old_speed_error = speed_error;
	if (order_current > 3.0)
		order_current = 3.0;
}
