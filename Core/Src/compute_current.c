/**
 * \file compute_current.c
 * \brief Ce fichier contient une fonction qui permet de calculer le courant à partir de la tension relevée par le capteur Arm_IRF540NBbFx
 * \author Paul-Etienne Rétaux
 * \date 30 octobre 2023
 *
 *
 */

#include "main.h"

float compute_current(uint32_t voltage){
	float current;
	current = (voltage*3300.0/4096-1650)/50;
	return current;
}
