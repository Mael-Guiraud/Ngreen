#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "struct.h"
#include "bruteforce.h"


void affiche_matrice(Graphe g)
{
	for(int i=0;i<g.N;i++)
	{
		for(int j=0;j<g.N;j++)
		{
			printf("%3d ",g.matrice[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


//Déclare une matrice remplie de 0 de taille N*N
int ** declare_matrice(int N)
{
	int ** matrice;
	assert(matrice = malloc(sizeof(int*)*N));
	for(int i = 0;i<N;i++)
	{
		assert(matrice[i]= calloc(N,sizeof(int)));
	}
	return matrice;
}


Graphe init_graphe(int N)
{
	Graphe g;
	g.N = N;
	g.matrice = declare_matrice(N);
	return g;
}
void libere_matrice(Graphe g)
{
	for(int i=0;i<g.N;i++)
	{
		free(g.matrice[i]);
	}
	free(g.matrice);
}

//prends un graphe vide en argument et le transforme en etoile
void graphe_etoile(Graphe g,int taille_liens)
{
	if(!(g.N%2)){printf("Impossible de générer une étoile avec un nombre pair de sommets\n");exit(5);}
	int pivot = g.N/2;
	int alea;
	for(int i=0;i<g.N;i++)
	{
		alea = rand()%taille_liens;
		g.matrice[pivot][i] = alea;
		g.matrice[i][pivot] = alea;
	}
	g.matrice[pivot][pivot] = 0;
}

//met toutes les cases du message a 1 dans la periode
void ecrire_message_periode(int * p,int periode, int debut, int fin)
{
	if(debut > fin)
	{
		for(int i=0;i<=fin;i++)p[i]=1;
		for(int i=debut;i<periode;i++)p[i]=1;
	}
	else
	{
		for(int i=debut;i<=fin;i++)p[i]=1;
	}
}
void affiche_periode(int *p, int periode)
{
	int actuel = 0;
	for(int i=0;i<periode;i++)
	{
		if(p[i]!=actuel)
		{
			if(actuel)printf("-%d[ ",i);
			else	  printf("[%d",i);
			actuel = 1-actuel;
		}
	}
	printf("\n");
}
void affiche_tab(int * tab, int taille)
{
	for(int i=0;i<taille;i++)
	{
		printf("%d ",tab[i]);
	}
	printf("\n");
}
void tri_bulles(int* tab,int taille)
{
	int sorted;
	int tmp;
	for(int i=taille-1;i>=1;i--)
	{
		sorted = 1;
		for(int j = 0;j<=i-1;j++)
		{
			if(tab[j+1]<tab[j])
			{
				tmp = tab[j+1];
				tab[j+1]= tab[j];
				tab[j]= tmp;
				sorted = 0;
			}
			if(sorted)return;
		}
	}
}

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
	for(int i =0;i<taille_message*(g.N/2)*3;i++)
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
		for(int j=0;j<=periode-taille_message;j++)
		{
			if(!aller[j] && !aller[(j+taille_message)%periode] && !retour[(j*taille_message+routes[i])%periode] && !retour[(j*taille_message+routes[i]+taille_message-1)%periode])
			{

				ecrire_message_periode(aller,periode,j,(j+taille_message)%periode);
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

int linear_prime(Graphe g,int taille_message)
{
	int result;
	for(int i =0;i<taille_message*(g.N/2)*3;i++)
	{
		result = algo_prime(g,i,taille_message);
		if(result != -1)
		{
			return result;
		}
	}
	return -1;
}

void simuls_periode(int nb_routes, int taille_message, int taille_routes,int nb_simuls)
{
	FILE * F = fopen("results_periode.data","w");
	Graphe g;
	long long int total_3NT, total_prime;


	for(int j = 1 ; j<=nb_routes;j++)
	{
		printf("Calculs pour %d routes: \n",j);
		
		total_3NT = 0;
		total_prime = 0;

		for(int i = 0;i<nb_simuls;i++)
		{
			g = init_graphe(j*2 + 1);
			graphe_etoile(g,j*taille_message);
			total_3NT += linear_3NT(g,taille_message);
			total_prime += linear_prime(g,taille_message);
			fprintf(stdout,"\rStep%5d /%d",i+1,nb_simuls);fflush(stdout);
			libere_matrice(g);
		}
		printf("\n");
		printf("P moyen (sur %d simulations) 3NT  : %lld\n",nb_simuls,total_3NT/nb_simuls);
		printf("P moyen (sur %d simulations) Prime: %lld\n",nb_simuls,total_prime/nb_simuls);


		
		fprintf(F, "%d %lld %lld %d %d\n",j,total_3NT/nb_simuls,total_prime/nb_simuls,j*taille_message,2*j*taille_message);
		printf("\n");
	}
	fclose(F);
}

void echec(int nb_routes, int taille_message,int taille_routes, int nb_simuls)
{
	FILE * F = fopen("results_echec.data","w");
	Graphe g;
	long long int total_3NT, total_brute, total_sl;


	for(int j = taille_message*nb_routes ; j<=3*taille_message*nb_routes;j+=100)
	{
		
		total_3NT = 0;
		total_brute = 0;
		total_sl = 0;

		for(int i = 0;i<nb_simuls;i++)
		{
			g = init_graphe(nb_routes *2 +1);
			graphe_etoile(g,taille_routes);
			if(algo_3NT(g,j,taille_message) != -1) total_3NT++;
			if(bruteforceiter(g,j,taille_message) != -1) total_brute++;
			if(algo_shortest_longest(g,j,taille_message)!= -1) total_sl++;
			libere_matrice(g);
			fprintf(stdout,"\rStep = %5d/%d [Period : %d]",i+1,nb_simuls,j);fflush(stdout);
		}

	
		fprintf(F, "%d %lld %lld %lld \n",j,total_sl,total_3NT,total_brute);

	}
	fclose(F);
}

void echec_taille_route(int nb_routes, int taille_message,int taille_routes, int nb_simuls)
{
	FILE * F = fopen("results_taille_route_echec.data","w");
	Graphe g;
	long long int total;

	for(int k = 1000;k<taille_routes;k+=1000)
	{
		for(int j = taille_message*nb_routes ; j<=3*taille_message*nb_routes;j+=100)
		{
			total = 0;

			for(int i = 0;i<nb_simuls;i++)
			{
				g = init_graphe(nb_routes *2 +1);
				graphe_etoile(g,k);
				if(algo_shortest_longest(g,j,taille_message)!= -1) total++;
				libere_matrice(g);
				fprintf(stdout,"\rStep = %5d/%d [Period : %d| Routes : %d]",i+1,nb_simuls,j,k);fflush(stdout);
			}

		
			fprintf(F, "%d %d %lld \n",j,k,total);

		}
	}
	fclose(F);
}
int main()
{
	srand(time(NULL));
	//simuls_periode(15,2500,10000,1000);
	echec_taille_route(8,2500,25000,1000);

	return 0;
}