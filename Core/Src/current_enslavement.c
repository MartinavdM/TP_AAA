/**
 * \file current_enslavement.c
 * \brief Ce fichier contient une fonction qui permet de calculer un rapport cyclique de consigne grâce à un correcteur PI et prenant en compte des échantillons de courant à un instant n et à un instant n-1 (préfixe "old_").
 * \author Paul-Etienne Rétaux
 * \date 30 octobre 2023
 *
 *
 */
#include "main.h"

extern float current;
extern float order_current;

float old_current_error = 0.0;
float current_error = 0.0;

float old_order_alpha = 0.0;
float order_alpha = 0.0;

void current_enslavement(void) {
	float b_zero = 0.86625; // k_p+k_I*T_e/2 = 0.8+2650*0.5*10^(-4)/2
	float b_one = -0.73375; // k_I*T_e/2 - k_p = 2650*0.5*10^(-4)/2 - 0.8
	order_alpha = old_order_alpha + b_zero * current_error
			+ b_one * old_current_error;
	current_error = order_current - current;
	old_order_alpha = order_alpha;
	old_current_error = current_error;
	if (order_alpha > 0.8)
		order_alpha = 0.8;
	if (order_alpha < 0.2)
		order_alpha = 0.2;
}
