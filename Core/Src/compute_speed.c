/**
 * \file compute_speed.c
 * \brief Ce fichier contient une fonction qui permet d'effectuer une moyenne de 100 échantillons de vitesse, celle-ci pouvant fluctuer de manière notable entre deux mesures.
 * \author Paul-Etienne Rétaux
 * \date 30 octobre 2023
 *
 *
 */

float compute_speed(float * speed_table)
{
	float sum = 0;
	for(int i=0;i<100;i++){
		sum+=speed_table[i];
	}
	return sum/100;
}
