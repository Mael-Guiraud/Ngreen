#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>


typedef struct graphe{
	int N; //nombre de sommets
	int ** matrice;//matrice d'adjacence pondérée
}Graphe ;

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

//prends un graphe vide en argument et le transforme en etoile
void graphe_etoile(Graphe g,int taille_liens)
{
	if(!g.N%2){printf("Impossible de générer une étoile avec un nombre pair de sommets\n");exit(5);}
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
	printf("on écrit de %d à %d \n",debut,fin);
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
			if(actuel)printf("fin d'un message en %d\n\n",i);
			else	  printf("debut d'un message en %d\n\n",i);
			actuel = 1-actuel;
		}
	}
}
int greedy_prime(Graphe g,int periode,int taille_message)
{
	if(!g.N%2){printf("G n'est peut être pas une étoile\n");exit(5);}
	int nb_routes = g.N/2;
	int routes[nb_routes];
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
		for(int j=0;j<periode-taille_message;j++)
		{
			if(!aller[j] && !aller[(j+taille_message-1)%periode] && !retour[(j+routes[i])%periode] && !retour[(j+routes[i]+taille_message-1)%periode])
			{

				ecrire_message_periode(aller,periode,j,(j+taille_message-1)%periode);
				ecrire_message_periode(retour,periode,(j+routes[i])%periode,(j+routes[i]+taille_message-1)%periode);
				break;
			}
		}

	}
	printf("aller\n");
	affiche_periode(aller,periode);
	printf("retour\n");
	affiche_periode(retour,periode);
	return 1;
}

int main()
{
	srand(time(NULL));
	Graphe g = init_graphe(5);
	affiche_matrice(g);
	graphe_etoile(g,700);
	affiche_matrice(g);
	greedy_prime(g,2500,500);
	free(g.matrice);
	return 0;
}