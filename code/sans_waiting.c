#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include "bruteforce.h"

//retourne -1 si la periode donnée ne donne pas de solutions, la taille de la periode sinon
int algo_shortest_longest(Graphe g,int periode,int taille_message)
{
	if(!(g.N%2)){printf("G n'est peut être pas une étoile\n");exit(5);}
	int nb_routes = g.N/2;
	int min = g.matrice[nb_routes][nb_routes+1];
	int max = min;
	for(int i=1;i<nb_routes;i++)
	{
		if(g.matrice[nb_routes][i+nb_routes+1] < min)
			min = g.matrice[nb_routes][i+nb_routes+1];
		if(g.matrice[nb_routes][i+nb_routes+1] > max)
			max = g.matrice[nb_routes][i+nb_routes+1];
	}
	if(periode >= (taille_message * nb_routes + 2*(max-min) ) ) 
		return taille_message * nb_routes + 2*(max-min);
	 
	return -1;
}

//retourne -1 si la periode donnée ne donne pas de solutions, la taille de la periode sinon
int algo_3NT(Graphe g,int periode,int taille_message)
{
	if(!(g.N%2)){printf("G n'est peut être pas une étoile\n");exit(5);}
	int nb_routes = g.N/2;
	int routes[nb_routes];
	int nb_slots = periode / taille_message;
	int assigned = 0;
	for(int i=0;i<nb_routes;i++)
	{
		routes[i]=2*g.matrice[nb_routes][i+nb_routes+1];
	}

	int aller[nb_slots], retour[periode];
	for(int i=0;i<nb_slots;i++){aller[i]=0;}
	for(int i=0;i<periode;i++){retour[i]=0;}


	//Pour chaque route
	for(int i=0;i<nb_routes;i++)
	{
		//On recherche le depart le plus petit
		for(int j=0;j<nb_slots;j++)
		{
			if(!aller[j] && !retour[(j*taille_message+routes[i])%periode] && !retour[(j*taille_message+routes[i]+taille_message-1)%periode])
			{

				ecrire_message_periode(aller,nb_routes,j,j);
				ecrire_message_periode(retour,periode,(j*taille_message+routes[i])%periode,(j*taille_message+routes[i]+taille_message-1)%periode);
				assigned = 1;
				break;
			}
		}
		if(!assigned)//Si une route n'a pas trouvé de départ possible, c'est que la fenètre est trop petite.
			return -1;
		assigned = 0;
	}
	return periode;
}


int linear_3NT(Graphe g,int taille_message)
{
	int result;
	int nb_routes = g.N/2;
	for(int i =nb_routes*taille_message;i<taille_message*(nb_routes)*3;i+=2500)
	{
		result = algo_3NT(g,i,taille_message);
		if(result != -1)
		{
			return result;
		}
	}
	return -1;
}


//retourne -1 si la periode donnée ne donne pas de solutions, la taille de la periode sinon
int algo_prime(Graphe g,int periode,int taille_message)
{
	if(!(g.N%2)){printf("G n'est peut être pas une étoile\n");exit(5);}
	int nb_routes = g.N/2;
	int routes[nb_routes];
	int assigned = 0;
	for(int i=0;i<nb_routes;i++)
	{
		routes[i]=2*g.matrice[nb_routes][i+nb_routes+1];
	}

	int aller[periode], retour[periode];
	for(int i=0;i<periode;i++){aller[i]=0;retour[i]=0;}


	//Pour chaque route
	for(int i=0;i<nb_routes;i++)
	{
		//On recherche le depart le plus petit
		for(int j=0;j<periode;j++)
		{
			if(!aller[j] && !aller[(j+taille_message)%periode] && !retour[(j+routes[i])%periode] && !retour[(j+routes[i]+taille_message-1)%periode])
			{

				ecrire_message_periode(aller,periode,j,(j+taille_message-1)%periode);
				ecrire_message_periode(retour,periode,(j+routes[i])%periode,(j+routes[i]+taille_message-1)%periode);
				assigned = 1;
				break;
			}
		}
		if(!assigned)//Si une route n'a pas trouvé de départ possible, c'est que la fenètre est trop petite.
			return -1;
		assigned = 0;
	}
	//affiche_periode(aller,periode);
	//affiche_periode(retour,periode);
//	printf("%d \n",periode);
	return periode;
}

int linear_prime(Graphe g,int taille_message)
{
	int result;
	int nb_routes = g.N/2;
	for(int i =nb_routes*taille_message;i<taille_message*(nb_routes)*3;i++)
	{
		result = algo_prime(g,i,taille_message);
		if(result != -1)
		{
			return result;
		}
	}
	return -1;
}

int linear_brute(Graphe g,int taille_message)
{
	int result;
	int nb_routes = g.N/2;
	for(int i =nb_routes*taille_message;i<taille_message*(nb_routes)*3;i++)
	{
		result = bruteforceiter(g,i,taille_message);
		if((result != -1)&&(result != -2))
		{
			return result;
		}
	}
	return -1;
}
