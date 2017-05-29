#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>


#include "struct.h"
#include "tests.h"
#define DEBUG 0

/* Structs For PAZL ------
------------------------*/


//Pour le new bruteforce
typedef struct{
	int id; // number of the message
  	int start_slot;// position in the bw_period
  	int return_slot;//position in the fw_period
}message;


typedef struct{
	int *fw_next;
	int *bw_next;
	int fw_margin;
	int bw_margin;
}stack;

// Pour le old bruteforce
typedef struct
{
  int debut;
  int fin;
  int suivant;
} intervalle_liste;



/* ALGOS GREEDY POLYNOMIAUX
----------------------*/



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




 /* Brute Force Rapide ----
 ------------------------*/


void print_solution(message *solution, int size, stack *s){
	printf("Partial solution of level %d:\n", size);
	printf("Forward margin : %d, Backward margin : %d\n",s[size].fw_margin,s[size].bw_margin);
	for(int i = 0; i <= size; i++){
		printf("[%d :(%d,%d) (%d,%d)] ",solution[i].id, solution[i].start_slot,s[size].fw_next[i], solution[i].return_slot, s[size].bw_next[i] );
	}
}

int add_route_fw(int message_size, int period, int route_number, int return_time, int previous_route, int added_route,
 message *solution, int level, stack* s){

//we try to place added_route right after previous route if possible in the period
//return 1 if the route can be added and modifies fw_period, bw_period and s

	//compute the position of added_route in the bw_period
	solution[level].return_slot = solution[previous_route].start_slot + message_size + return_time;
	if (solution[level].return_slot > period) solution[level].return_slot -= period; //put retour in the interval modulo period
	
	//Test wether the message collides another in the bw_period
	for(int i = 0; i < level; i++){
		if (solution[level].return_slot > solution[i].return_slot - message_size && solution[level].return_slot < solution[i].return_slot + message_size) return 0;
	}
	if(solution[level].return_slot > period - message_size) return 0; //special case for the first message

	//there are no collision, we add the route in the solution 
	solution[level].id = added_route;
	solution[level].start_slot = solution[previous_route].start_slot + message_size;

	//we update the stack so that no routes can be glued to another if it has an id larger than add_route
	memcpy(s[level].fw_next,s[level-1].fw_next,sizeof(int)*level);
	memcpy(s[level].bw_next,s[level-1].bw_next,sizeof(int)*level);
	//update the next_route
	for(int i = 0; i < level; i++){
		if(s[level].fw_next[i] <= added_route) s[level].fw_next[i] = added_route + 1;
		if(s[level].bw_next[i] <= added_route) s[level].bw_next[i] = added_route + 1;
	}
	//one cannot use previous route anymore
	s[level].fw_next[previous_route] = route_number;

	int min = 0, max = period;
	int previous = 0;
	for(int i = 1; i < level; i++){
		if(solution[i].return_slot < solution[level].return_slot && solution[i].return_slot > min) {
			min = solution[i].return_slot;
			previous = i;
		}
		if(solution[i].return_slot > solution[level].return_slot && solution[i].return_slot < max) max = solution[i].return_slot;
	}

	//we compute the change of margin 
	s[level].bw_margin = s[level -1].bw_margin + (max - solution[level].return_slot)/message_size 
	+ (solution[level].return_slot - min)/message_size - (max - min)/message_size ;
	if(s[level].bw_margin < 0) return 0;//cut when there are not enough room in one of the period for the remaining route
	//we should put that before to cut earlier

	//compute wheher we cannot put something before or after route_added
	if(solution[level].return_slot + 2*message_size > max ) {
		s[level].bw_next[level] = route_number; //cannot put something after this element
	}
	else{
		s[level].bw_next[level] = 1;
	}
	if(min + 2*message_size > solution[level].return_slot) s[level].bw_next[previous] = route_number;

	//find the message after the one we are inserting in the fw_period
	max = period;
	for(int i = 1; i < level; i++){
		if(solution[i].start_slot > solution[level].start_slot && solution[i].start_slot < max) max = solution[i].start_slot;
	}
	if(max - solution[level].start_slot < 2*message_size) {
		s[level].fw_next[level] = route_number; //cannot put something after this element
	}
	else{
		s[level].fw_next[level] = 1;
	}
	s[level].fw_margin = s[level -1].fw_margin; //does not change since the packet are glued together in the fw windows

	return 1;
}


int add_route_bw(int message_size, int period, int route_number, int return_time, int previous_route, int added_route,
 message *solution, int level, stack* s){

//we try to place added_route right after previous route if possible in the period
//return 1 if the route can be added and modifies fw_period, bw_period and s

	//compute the position of added_route in the bw_period
	solution[level].start_slot = solution[previous_route].return_slot + message_size - return_time;
	if (solution[level].start_slot < period) solution[level].start_slot += period; //put retour in the interval modulo period
	
	//Test wether the message collides another in the fw_period
	for(int i = 0; i < level; i++){
		if (solution[level].start_slot > solution[i].start_slot - message_size && solution[level].start_slot < solution[i].start_slot + message_size) return 0;
	}
	if(solution[level].start_slot > period - message_size) return 0; //special case for the first message

	//there are no collision, we add the route in the solution 
	solution[level].id = added_route;
	solution[level].return_slot = solution[previous_route].return_slot + message_size;

	//we update the stack so that no routes can be glued to another if it has an id larger than add_route
	memcpy(s[level].fw_next,s[level-1].fw_next,sizeof(int)*level);
	memcpy(s[level].bw_next,s[level-1].bw_next,sizeof(int)*level);
	//update the next_route
	for(int i = 0; i < level; i++){
		if(s[level].fw_next[i] <= added_route) s[level].fw_next[i] = added_route + 1;
		if(s[level].bw_next[i] <= added_route) s[level].bw_next[i] = added_route + 1;
	}
	//one cannot use previous route anymore in the bw_period, but it can add anything in the fw
	s[level].bw_next[previous_route] = route_number;

	//find the message after the one we are inserting in the bw_period
	int max = period;
	for(int i = 1; i < level; i++){
		if(solution[i].return_slot > solution[level].return_slot && solution[i].return_slot < max) max = solution[i].return_slot;
	}
	if(max - solution[level].return_slot < 2*message_size) {
		s[level].bw_next[level] = route_number; //cannot put something after this element
	}
	else{
		s[level].bw_next[level] = 1;	
	}
	s[level].bw_margin = s[level -1].bw_margin; //does not change since the packet are glued together in the fw windows


	int min = 0; 
	max = period;
	int previous = 0;
	for(int i = 1; i < level; i++){
		if(solution[i].start_slot < solution[level].start_slot && solution[i].start_slot > min) {
			min = solution[i].start_slot;
			previous = i;
		}
		if(solution[i].start_slot > solution[level].start_slot && solution[i].start_slot < max) max = solution[i].start_slot;
	}

	//we compute the change of margin 
	s[level].fw_margin = s[level -1].fw_margin + (max - solution[level].start_slot)/message_size 
	+ (solution[level].start_slot - min)/message_size - (max - min)/message_size ;
	if(s[level].fw_margin < 0) return 0;//cut when there are not enough room in the period for the remaining routes

	//compute wheher we cannot put something before or after route_added
	if(solution[level].start_slot + 2*message_size > max ) {
		s[level].fw_next[level] = route_number; //cannot put something after this element
	}
	else{
		s[level].fw_next[level] = 1;	
	}
	if(min + 2*message_size > solution[level].start_slot) s[level].fw_next[previous] = route_number;
	return 1;
}


int recursive_search(message *solution, stack *s, int *used_route, int level, int* return_time, int route_number, int message_size, int period){ //renvoie 1 si une solution a été trouvée, 0 sinon
	//level is the current depth in the search tree, it corresponds to solutions with level+1 elements 
	//print_solution(solution,level-1,s);
	if(level == route_number ) return 1; //we have placed all messages
	//place a message next to another one in the forward windows
	for(int i = 0; i < level; i++){//go through the  messages already placed 
		for(int j = s[level-1].fw_next[i]; j < route_number; j++ ){//add a message next to the message number i in the forward windows
			if( !used_route[j] && add_route_fw(message_size, period, route_number, return_time[j], i, j, solution, level, s)){
				used_route[j] = 1;
				if(recursive_search(solution,s,used_route,level+1,return_time,route_number, message_size,period)) return 1; // we have found a solution, exit
					//we have not found a solution go one level back in the tree
				used_route[j] = 0;
			}
		}
		//place a message next to another one in the backward windows
		for(int j = s[level-1].bw_next[i]; j < route_number; j++ ){//add a message next to the message number i in the backward windows
			if( !used_route[j] && add_route_bw(message_size, period, route_number, -return_time[j] + period, i, j, solution, level, s)){
				used_route[j] = 1;
				if(recursive_search(solution,s,used_route,level+1,return_time,route_number, message_size,period)) return 1; // we have found a solution, exit
					//we have not found a solution go one level back in the tree
				used_route[j] = 0;
			}
		}
	}
	return 0;
}


int search(Graphe g,int message_size, int period){
	

	int route_number = g.N/2;
	int return_time[route_number];

	for(int i = 0; i < route_number; i++)return_time[i]= (2 * g.matrice[route_number][i + route_number + 1]) %period;
	if(DEBUG)printf("Instance aléatoire :\n");//we assume that the value in return time are in [0,period[
	if(DEBUG)for(int i = 0; i < route_number; i++)printf("%d  ",return_time[i]);
	int shift = return_time[0];
	if(DEBUG)printf("\nInstance aléatoire décalée :\n");
	for (int i = 0; i < route_number; i++){//we shift the values so that the first route has return time 0
		return_time[i] -= shift;
		if (return_time[i] < 0) return_time[i] += period;
		if(DEBUG)printf("%d  ",return_time[i]);
	}
	
    /* Memory allocation */		
	stack *s = malloc(sizeof(stack)*route_number);
  	for(int i = 0; i < route_number; i++){ 
  		s[i].fw_next = malloc(sizeof(int)*route_number);
  		s[i].bw_next = malloc(sizeof(int)*route_number);
  	}
  	message *solution = malloc(sizeof(message)*route_number);
  	int *used_route = calloc(route_number,sizeof(int));

  	/* Initialization, the first route is fixed */
  	
  	s[0].fw_next[0] = 1;
  	s[0].bw_next[0] = 1;
  	s[0].fw_margin = period /message_size - route_number;
  	s[0].bw_margin = period /message_size - route_number;
  	solution[0].id = 0;
  	solution[0].start_slot = 0;
  	solution[0].return_slot = 0;
  	
  	int return_value;
  	/* Call the recursive part with the proper algorithm */
  	if(recursive_search(solution, s, used_route, 1, return_time, route_number, message_size, period)){
  		//printf("Solution trouvée \n");
  		//print_solution(solution,route_number-1,s);
  		return_value =1;
  	}
  	else{
  		return_value = -1;
  	}
  	/* Free the memory */
  	free(solution);
  	free(used_route);
  	for(int i = 0; i < route_number ; i++) {
  		free(s[i].fw_next);
  		free(s[i].bw_next);
  	}
  	free(s);
  	return return_value;
}


/* Ancien BruteForce


-----------------------*/




//intervalle fermé à gauche et ouvert à droite
// le codage debut, taille serait sans doute légèrement mieux

intervalle_liste *initialise (int nbr_route, int taille_paquets, int periode)
{       //initialise les intervalles de temps restants pour le retour
  intervalle_liste *l = malloc (sizeof (intervalle_liste) * nbr_route);
  l[0].suivant = -1;
  l[0].debut = taille_paquets;
  l[0].fin = periode;
  return l;
}

void affiche_intervalle (intervalle_liste * liste)
{
  int pos = 0;
  while (pos != -1)
    {
      printf ("[%d,%d[  ", liste[pos].debut, liste[pos].fin);
      pos = liste[pos].suivant;
    }
  printf ("\n");
}


int ajoute_element (intervalle_liste * liste, int debut, int taille,
    int taille_paquets, int *nombre_slot, int nbr_route)
{       //renvoie 0 si l'insertion n'est pas possible, 1 sinon et modifie alors liste
  //pour couper en deux l'intervalle approprié
  int pos = 0;
  while (pos != -1)
    {       //détecte l'intervalle à découper si il existe
      if (liste[pos].debut <= debut
    && liste[pos].fin - debut >= taille_paquets)
    {     //l'intervalle existe, on le découpe
    //on calcule le nombre de slot après décooupage (en prenant en compte les paquets déjà placés)
    *nombre_slot +=
      (debut - liste[pos].debut) / taille_paquets + (liste[pos].fin -
                 debut) /
      taille_paquets - (liste[pos].fin -
            liste[pos].debut) / taille_paquets;
    if (*nombre_slot >= nbr_route)
      {     // si le nombre de slots retour est insuffisant, on ne permet pas l'ajout de l'élément à cet endroit     
        liste[taille].debut = debut + taille_paquets;
        liste[taille].fin = liste[pos].fin;
        liste[taille].suivant = liste[pos].suivant;
        liste[pos].fin = debut;
        liste[pos].suivant = taille;
        return 1;
      }
    else
      {
        *nombre_slot += 1;
        return 0;
      }

    }
      pos = liste[pos].suivant;
    }
  return 0;
}

void
retire_element (intervalle_liste * liste, int debut, int taille,
    int *nombre_slot, int taille_paquets)
{       //enlève une route et fusionne les intervalles correspondants
  int i;
  for (i = 0; i < taille; i++)
    {
      if (liste[i].fin == debut)
  {
    break;
  }
    }       //cherche l'intervalle à fusionner, l'autre est en dernière position par construction
  *nombre_slot +=
    -(debut - liste[i].debut) / taille_paquets - (liste[taille].fin -
              debut) / taille_paquets +
    (liste[taille].fin - liste[i].debut) / taille_paquets;
  liste[i].suivant = liste[taille].suivant;
  liste[i].fin = liste[taille].fin;
}

int prochain_debut (intervalle_liste * liste, int debut, int taille, int taille_paquets)
{       //renvoie la prochaine position possible pour la route (décalage à la position actuelle donnée par debut)
  int pos = 0;
  while (liste[pos].suivant != -1){
    if (debut < liste[pos].fin){
      break;
    }
    pos = liste[pos].suivant;
  }       //trouve le premier intervalle qui peut contenir la route
  if (debut < liste[pos].debut)
    {       //debut avant l'intervalle
      if (liste[pos].fin - liste[pos].debut >= taille_paquets)
  {     //on met la route au début de l'intervalle si assez de place
    return liste[pos].debut - debut;
  }
    }
  else
    {
      if (debut < liste[pos].debut + taille_paquets)
  {     //debut dans une partie proche du début de l'intervalle 
    if (liste[pos].fin - liste[pos].debut >= 2 * taille_paquets)
      {     // Evite les intervalles inférieur à taille_paquet qui sont inutiles
        return liste[pos].debut + taille_paquets - debut;
      }
  }
      else
  {     //si il y a la place on décale juste de 1
    if (liste[pos].fin - debut >= taille_paquets)
      {
        return 1;
      }
  }
    }
  //dans les cas restants on renvoie le début du premier intervalle qui peut contenir une route

  while (liste[pos].suivant != -1)
    {
      pos = liste[pos].suivant;
      if (liste[pos].fin - liste[pos].debut >= taille_paquets)
  {
    return liste[pos].debut - debut;
  }
    }
  return liste[pos].fin - debut + taille_paquets; //on a pas trouvé de bon emplacement, on retourne au début des temps retour 
}


void
print_sol (int *solution_pos, int *solution_num, int nbr_route, int budget)
{
  printf ("Budget restant: %d \n", budget);
  for (int i = 0; i < nbr_route; i++)
    {
      printf ("(%d,%d) ", solution_num[i], solution_pos[i]);
    }
  printf ("\n");
}


//mettre debut_courant à la place de décalage_courant partout ? Ca pourrait simplifier un peu
//on pourrait utiliser un entier dont les bits représente les routes restantes et trouver la première route libre avec une instruction processeur spéciale

int bruteforceiter (Graphe g, int periode, int taille_paquets)
{
  long long int compteur = 0;

  if (!(g.N % 2))
    {
      printf ("G n'est peut être pas une étoile\n");
      exit (5);
    }

  int nbr_route = g.N / 2;
  int temps_retour_param[nbr_route];

  for (int i = 0; i < nbr_route; i++)
    {
      temps_retour_param[i] = 2 * g.matrice[nbr_route][i + nbr_route + 1];
    }


  int *solution_pos, *solution_num, *temps_retour, *route_restante;
  assert (solution_pos = malloc (sizeof (int) * nbr_route));
  assert (solution_num = malloc (sizeof (int) * nbr_route));
  assert (temps_retour = malloc (sizeof (int) * nbr_route));
  assert (route_restante = malloc (sizeof (int) * nbr_route));
  //affiche les données sur lesquelles on lance le bruteforce

  temps_retour[0] = temps_retour_param[0];
  for (int j = 1; j < nbr_route; j++)
    {
      route_restante[j] = 1;  //on pourrait juste virer la première route ?
      temps_retour[j] = temps_retour_param[j];

    }


  /*
     for(int j = 0; j < nbr_route; j++){
     printf("%d(%d)",temps_retour[j],temps_retour_param[j]);
     }
     printf("\n"); */
  route_restante[0] = 0;
  solution_pos[0] = 0;
  solution_num[0] = 0;
  int solution_taille = 1;  //première route fixée
  int budget = periode - nbr_route * taille_paquets;
  int num_courant = 1, decalage_courant = 0;
  intervalle_liste *retour = initialise (nbr_route, taille_paquets, periode);
  int debut_retour, i;
  int nombre_slot = periode / taille_paquets;

  //normalise les temps retour en mettant le premier à 0
  for (int j = nbr_route - 1; j >= 0; j--)
    temps_retour[j] = (temps_retour[j] - temps_retour[0] + periode) % periode;
  //affiche les données sur lesquelles on lance le bruteforce
/* printf("\n");
  for(int j = 0; j < nbr_route; j++){
    printf("%d(%d)",temps_retour[j],temps_retour_param[j]);
  }
  printf("\n");*/
  /////////////////// Début de l'arbre de recherche ////////////////////////////


  //while (compteur < 500000000)
    while (1)
    {
      compteur++;
      if (solution_taille == nbr_route)
      {
        //print_sol(solution_pos,solution_num,nbr_route,budget);
        //printf("PERIODE %d \n",periode);

        free (solution_num);
        free (solution_pos);
        free (route_restante);
        free (temps_retour);
        return periode;
      }     //sortir la solution et arrêt
      if (num_courant == nbr_route)
      {     //plus de route à utiliser, on revient en arrière et on utilise un début de route plus élevé
        solution_taille--;
        if (solution_taille == 0)
          {
            free (solution_num);
            free (solution_pos);
            free (route_restante);
            free (temps_retour);

            return -1;
          }
        num_courant = solution_num[solution_taille];
        route_restante[num_courant] = 1;
        debut_retour = solution_pos[solution_taille] + temps_retour[num_courant];
        if (debut_retour >= periode)
          {
            debut_retour -= periode;  // valeur au plus deux fois la période
          }
        //printf("Taille %d, debut %d \n",solution_taille, debut_retour);
        //affiche_intervalle(retour);
        retire_element (retour, debut_retour, solution_taille, &nombre_slot,
            taille_paquets);
        //affiche_intervalle(retour);
        decalage_courant = solution_pos[solution_taille] - solution_pos[solution_taille - 1] - taille_paquets;  //decalage de la route qu'on enlève
        //de la solution avec la précédente
        budget += decalage_courant;
        decalage_courant += prochain_debut (retour, debut_retour, solution_taille, taille_paquets); //calcul le prochain point ou on va placer le paquet de num_courant 
      }
      else
      {
        if (budget - decalage_courant < 0)
          {     // plus de budget pour la route, on essaye de mettre la route suivante à la place
            for (i = num_courant + 1; i < nbr_route && !route_restante[i];
           i++){}
            num_courant = i;
            decalage_courant = 0;
          }
        else
          {
                debut_retour =solution_pos[solution_taille - 1] + taille_paquets +  decalage_courant + temps_retour[num_courant];
                if (debut_retour >= periode)
              {
                debut_retour -= periode;  // valeur au plus deux fois la période
              }
                if (ajoute_element(retour, debut_retour, solution_taille, taille_paquets,&nombre_slot, nbr_route))
                { //on agrandit ici la solution partielle d'une nouvelle route
                  solution_pos[solution_taille] = solution_pos[solution_taille - 1] + decalage_courant + taille_paquets;
                  solution_num[solution_taille] = num_courant;
                  budget -= decalage_courant;
                  solution_taille++;
                  route_restante[num_courant] = 0;
                  for (i = 0; i < nbr_route && !route_restante[i]; i++)
                    {
                    }
                  num_courant = i;  //on utilise la première route libre comme nouveau candidat à placer
                  decalage_courant = 0; //avec un décalage de 0
                  //print_sol(solution_pos,solution_num,solution_taille,budget);
                  //affiche_intervalle(retour);
                }
                else
                {   //pas de place pour le retour, on essaye le décalage suivant.
                  decalage_courant += prochain_debut (retour, debut_retour, solution_taille,taille_paquets);
                }
          }
      }
    }
  //printf("Pas de solution \n");
  free (solution_num);
  free (solution_pos);
  free (route_restante);
  free (temps_retour);

  return -2;
}


/*RECHERCHES LINEAIRES 
TODO : pointeurs de fonctions ?
-----------------*/




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

int linear_search(Graphe g,int taille_message)
{
	int result;
	int nb_routes = g.N/2;
	for(int i =nb_routes*taille_message;i<taille_message*(nb_routes)*3;i++)
	{
		result = search(g, taille_message,i);
		if(result != -1)
		{
			return i;
		}
	}

	return -1;
}
