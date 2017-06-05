#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "simulations.h"

#include "struct.h"
#include "init.h"
#include "random.h"
int main()
{
	srand(time(NULL));

	/*****
	Effectue une recherche linéaire pour les algos PAZL afin de trouver la plus petite periode moyenne
	*****/
	//simuls_periode_PAZL(7,2500,20000,100);

	/******************************************/	

	/*****
	Taux de reussite de chaque algo PAZL en fonction de la periode
	*****/
	//echec_PAZL(8,2500,30000,1000);


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
	Taux de reussite de départ PALL avec un retour GP sur une periode donnée (on fait varier la marge)
	*****/
	//sucess_aller_PALL(8,2500,20000,3000,2000,21000);





	/******************************************/


	/*****
	Taux de reussite des algos PALL avec un aller random sur une periode donnée (on fait varier la marge)
	*****/

	//sucess_retour_PALL(8,2500,20000,3000,1000,21000);


	/******************************************/

	/*****
	//Taux de reussite des diferents nombres d'instances générées
	*****/

	nombre_random_PALL(8,2500,20000,1000, 20000);

	/*****
	Tmax moyen pour sto vs sp
	*******/

	//marge_PALL_stochastique(8,2500,20000,1000,50000);


	return 0;
}
