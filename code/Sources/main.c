#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "simulations.h"


int main()
{
	srand(time(NULL));

	/*****
	Effectue une recherche linéaire pour les algos PAZL afin de trouver la plus petite periode moyenne
	*****/
	//simuls_periode_PAZL(12,2500,20000,100);

	/******************************************/	

	/*****
	Taux de reussite de chaque algo PAZL en fonction de la periode
	*****/
	echec_PAZL(8,2500,30000,1000);


	/******************************************/


	/*****
	Fichiers en 3D pour PALL
	*****/
	/*
	for(int i=3;i<5;i++)
	succes_PALL_3D(8,2500,5000, 1000,i);
	*/

	/******************************************/
	
	/*****
	Taux de reussite de chaque algo PALL sur une periode donnée (on fait varier la marge)
	*****/
	// sucess_PALL_fixed_period(8,2500,20000,3000,10000,30000);






	return 0;
}