#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "struct.h"

typedef struct element{
	int index;
	int release;
	int deadline;
	struct element * next;
} Element;

typedef struct ensemble{
	int numero_element; // vaut -1 si c'est un ensemble
	int temps_depart;
	struct ensemble * filsG;
	struct ensemble * frereG;
	struct ensemble * frereD;
} Ensemble;


Ensemble * crisis(Ensemble * ens,Element * crisise, Element * elemspere,Element * touselems, int taille_paquet);
Ensemble * init_ensemble(){return NULL;}
Element * init_element(){return NULL;}

//Retourne l'indice du min d'un tableau d'int
int lower(int * tab,int taille)
{
	int i;
	int min = tab[0];
	int indice_min = 0;
	for(i=1;i<taille;i++)
	{
		if(tab[i] < min)
		{
			min = tab[i];
			indice_min = i;
		}
		
	}
	return indice_min;
}
int max(int a, int b)
{
	if(a>b)
		return a;
	return b;
}
int min(int a,int b)
{
	if(a<b)
		return a;
	return b;
}
//Retourne le max tableau d'int
int greater(int * tab,int taille)
{
	int i;
	int max = tab[0];
	int indice_max = 0;
	for(i=1;i<taille;i++)
	{
		if(tab[i] > max)
		{
			max = tab[i];
			indice_max = i;
		}
		
	}
	return indice_max;
}


void affiche_ensemble_unique(Ensemble * ens)
{
	if(!ens)
	{
		printf("VIDE\n");return;
	}
	printf("(%p) num %d / date %d /fils g %p /frereG %p /frereD %p\n",ens,ens->numero_element,ens->temps_depart,ens->filsG,ens->frereG,ens->frereD);
}
void affiche_ensemble(Ensemble * ens)
{
	//affiche_1_ensemble(ens);
	if(ens == NULL)
	{
		printf("ENSEMBLE VIDE");
		return;
	}
	if(ens->numero_element == -1) // si c'est un ensemble
	{
		//si c'est un ensemble, par définition il a forcement un fils gauche
		//qui est soit un élément, soit un ensemble, donc appel recursif sur le fils gauche
		printf("Sous ensemble(%d){ ",ens->temps_depart);
		affiche_ensemble(ens->filsG);
		printf("} ");
		
	}
	else //si c'est un élément
	{
		printf("%d(%d) ",ens->numero_element,ens->temps_depart);
	}
	ens = ens->frereD;
	while(ens)
	{
		if(ens->numero_element == -1)
		{
			affiche_ensemble(ens);
			return;
		}
		else
		{
			printf("%d(%d) ",ens->numero_element,ens->temps_depart);
		}
		ens = ens->frereD;
	}
	
}
Element * ajoute_elemt(Element * elem,int index, int release, int deadline)
{
	Element * tmp = elem;
	Element * new = malloc(sizeof(Element));
	new->index = index;
	new->release = release;
	new->deadline = deadline;
	new->next = NULL;
	if(elem == NULL)//0 element
	{
		return new;
	}

	//test pour l'element 1
	if(elem->release > release)//inser avant
	{
		new->next = elem;
		return new;
	}

	while(elem->next)
	{
		
		if(elem->next->release > release)//inser avant
		{
			new->next = elem->next;
			elem->next = new;
			return tmp;
		}
		elem = elem->next;
	}
	elem->next = new;
	return tmp;
}
Ensemble * cree_ensemble(int numero, int temps)
{
	Ensemble * ens = malloc(sizeof(Ensemble));
	ens->numero_element = numero;
	ens->temps_depart = temps;
	ens->filsG = NULL;
	ens->frereG = NULL;
	ens->frereD = NULL;
	return ens;
}


Element* retire_element_i(Element * elem, int index)
{
	if(elem == NULL)
	{
		printf("aucun element");
		return NULL;
	}
	Element * tmp;
	Element * debut = elem;
	if(elem->index == index)//cas du premier element
	{
		tmp = elem;
		debut = elem->next;
		free(tmp);
		return debut;
	}
	while(elem->next)
	{
		if(elem->next->index == index)
		{
			tmp = elem->next;
			elem->next = elem->next->next;
			free(tmp);
			return debut;
		}
		elem = elem->next;
	}
	return debut;
}
Element * get_element_i(Element * elem, int index)
{
	while(elem)
	{
		if(elem->index == index)
		{
			//printf("%d %d\n",elem->index,index);
			return elem;
		}
		elem = elem->next;
	}
	return NULL;
}
void freeelems(Element * elem)
{
	
	Element * tmp;
	while(elem)
	{
		tmp = elem;
		elem=elem->next;
		free(tmp);
	}
}
void affichejobs(Element* elem)
{
	if(!elem)
		printf("aucun element");
	//printf("Elements\n");
	while(elem)
	{
		printf("%d(%d,%d),  ",elem->index, elem->release, elem->deadline); 
		elem = elem->next;
	}
	printf("\n");
}
Element * cpy_elems(Element * elems)
{
	Element * new = init_element();
	while(elems)
	{
		new = ajoute_elemt(new,elems->index, elems->release,elems->deadline);
		elems = elems->next;
	}
	return new;
}
int date_fin(Ensemble * ens,int taille_paquet)
{
	while(ens->frereD)
	{
		ens = ens->frereD;
	}
	//affiche_ensemble_unique(ens);
	if(ens->numero_element == -1)
	{
		return date_fin(ens->filsG,taille_paquet);
	}
	else
	{
		return ens->temps_depart+taille_paquet;
	}
}
//renvoie l'element eligible
int eligible(Element * elems, int time)
{

	Element * debut =elems;
	if(elems == NULL)
		return -1;
	//printf("pointeur envoyé = %p\n",elems);
	int deadline_min=INT_MAX;
	int elu = -1;
	while(elems)
	{
		if(elems->release <= time)//si dispo
		{ 
			if(elems->deadline < deadline_min)
			{
				deadline_min = elems->deadline;
				elu = elems->index;
			}
		}
		elems = elems->next;
	}
	if(elu != -1)
	{
		
		return elu;
	}
	else
	{
		
		
		return debut->index;//la liste est triée par release time, donc si on ne trouve pas d'eligible, on saute direct au premier release
	}
}
void libereens(Ensemble * ens)
{
	if(ens)
	{
		if(ens->numero_element == -1)
		{
			libereens(ens->filsG);
		}
		libereens(ens->frereD);
		free(ens);
		ens = NULL;
	}
}
Ensemble * cpyens(Ensemble * ens)
{
	if(ens == NULL)
		return NULL;
	Ensemble * cpy = NULL;
	cpy = cree_ensemble(ens->numero_element,ens->temps_depart);
	if(ens->numero_element == -1) // si c'est un ensemble
	{
		cpy->filsG = cpyens(ens->filsG);
	}
	cpy->frereD = cpyens(ens->frereD);
	if(cpy->frereD)
		cpy->frereD->frereG = cpy;
	return cpy;
}
Ensemble * invade(Ensemble * ens,Element * touselems,int depart,int taille_paquet)
{

	Ensemble * ens2 = NULL;
	Ensemble * tmp = NULL;
	
	printf("ENTREE DANS LA FONCTION INVADE date %d-------------------------------\n",depart);
	//affiche_ensemble(ens);printf("\n");
	//on se place a droite de l'ensemble qui a crisis
	while(ens->frereD)
	{
		ens = ens->frereD;
	}
	Element * elems = init_element();
	Element * elemtmp=NULL;
	int date = depart;
	//on backtrack jusqu'a la fin
	while(ens)
	{
		if(ens->numero_element != -1) // si c'est un element
		{
			elemtmp = get_element_i(touselems,ens->numero_element);
			if(ens2 == NULL)//initialisation
			{
				ens2 = cree_ensemble(-2,-1);
			}
			else
			{
				tmp = ens2;
				ens2->frereG = cree_ensemble(-2,-1);
				ens2 = ens2->frereG;
				ens2->frereD = tmp;
			}
			elems=ajoute_elemt(elems,elemtmp->index,elemtmp->release,elemtmp->deadline);
			//printf("Ajout de %d dans elems \n",elemtmp->index);
			//affichejobs(elems);
		}

		else //si c'est un ensemble
		{
			if(ens2== NULL)
			{
				ens2 = cpyens(ens);
			}
			else
			{
				ens2->frereG = cpyens(ens);
				ens2->frereG->frereD = ens2;
				ens2 = ens2->frereG;
			}
		}
		if(ens->frereG)//on regarde si on est pas au debut de l'ensemble
		{
			
			//printf("On ajoute ");affiche_ensemble(ens);printf("au r.s\n");
			ens = ens->frereG;
			libereens(ens->frereD);
			ens->frereD = NULL;
		}
		else
		{
			//printf("On ajoute ");affiche_ensemble(ens);printf("au r.s\n");
			libereens(ens);
			ens = NULL;
		}
	}
	//affiche_ensemble(ens2);printf(" R.S.\n");
	//affichejobs(elems);
	
	Ensemble* ens3 = NULL;
	int i;

	while(ens2)//parcours de ens2
	{
		if(ens2->numero_element == -2)//si on attend un element
		{
			
			i = eligible(elems,date);
			if(i==-1)
			{
				//printf("Failure 1\n");
				return NULL;
			}
			elemtmp = get_element_i(touselems,i);
			if(elemtmp == NULL)
			{
				//printf("Failure 0\n");
				return NULL;
			}
			
			date = max(date,elemtmp->release);
			if(date+taille_paquet > elemtmp->deadline)//CRISIS
			{
				//printf("Crisis(invade) sur la tache %d\n",elemtmp->index);
				Element * crisisrec = ajoute_elemt(NULL,elemtmp->index,elemtmp->release,elemtmp->deadline);
				elems=retire_element_i(elems,crisisrec->index);
				if(!elems)
					elems = ajoute_elemt(elems,99999,99999,999999);
				ens3 = crisis(ens3,crisisrec,elems,touselems,taille_paquet);
				//printf("-------------\n\n\n");
				//affichejobs(elems);
				//affiche_ensemble(ens2);		printf("\n");
				//affiche_ensemble(ens3);		
				//printf("-------------\n\n\n");		
				if(ens3 == NULL)
				{
					//printf(" NO PULL(%d) FOUND(invade)\n",elemtmp->index);
					return NULL;
				}
				while(ens3->frereD)
					ens3 = ens3->frereD;
				//affiche_ensemble(ensembletmp);printf("Elemenent le plus a droite\n");
				date = date_fin(ens3,taille_paquet);
				
				
				//rajouter un element pour rescheduler PULL(X) apres la crisis dans l'invasion
				if(ens2 == NULL)//initialisation
				{
					ens2 = cree_ensemble(-2,-1);
				}
				else
				{
					tmp = ens2;
					ens2->frereG = cree_ensemble(-2,-1);
					ens2 = ens2->frereG;
					ens2->frereD = tmp;
				}
			}
			else// PAS DE CRISE
			{
				
				if(ens3 == NULL)//premier element de ens3
				{
					ens3=cree_ensemble(elemtmp->index,date);
					
				}
				else
				{
					
					ens3->frereD=cree_ensemble(elemtmp->index,date);
					ens3->frereD->frereG = ens3;
					ens3 = ens3->frereD;
				}
				elems= retire_element_i(elems,elemtmp->index);
				//printf("on Schedule %d a la date %d et on le retire de elem",elemtmp->index,date);
				date+=taille_paquet;
				
				if(ens2->frereD)//Si il y a un prochain element
				{
					if(ens2->frereD->numero_element == -1)//et que c'est un ensemble
					{
						if(date+taille_paquet>=ens2->frereD->temps_depart)//ET que ca INVADE
						{
							//printf("%d INVADE date %d\n",elemtmp->index,date);
							ens2->frereD->filsG = invade(ens2->frereD->filsG,touselems,date+taille_paquet,taille_paquet);
							ens2->frereD->temps_depart = ens->frereD->filsG->temps_depart;
							
						}
					}
				}
			}
			//affiche_ensemble(ens3);printf(" Element\n");
		}
		else//si c'est un ensemble
		{
			
			if(ens3== NULL)
			{
				
				ens3 =  cree_ensemble(-1,ens2->temps_depart);
				ens3->filsG = cpyens(ens2->filsG);
				
			}
			else
			{
				ens3->frereD = cree_ensemble(-1,ens2->temps_depart);
				ens3->frereD->filsG = cpyens(ens2->filsG);
				ens3->frereD->frereG = ens3;
				ens3 = ens3->frereD;
			}
			//affiche_ensemble(ens2);
			date = date_fin(ens3,taille_paquet);//printf("fin = %d \n",date);

		}
		tmp = ens2;
		ens2 = ens2->frereD;
		free(tmp);
		
	}

	//on remet ens3 a son debut
	
	while(ens3->frereG)
		ens3 = ens3->frereG;
	//affiche_ensemble(ens3);printf("RETOUR INVADE --------------------\n");
	return ens3;
		
}
Ensemble * crisis(Ensemble * ens,Element * crisise, Element * elemspere,Element * touselems, int taille_paquet)
{
	printf("------------------ENTREE DANS LA FONCTION CRISIS-----------------------------\n");
	if(ens == NULL)
		return NULL;
	printf("ens != NULL\n");
	//affiche_ensemble(ens);printf("\n");
	Ensemble * ens2 = NULL;
	Ensemble * tmp = NULL;
	Ensemble * debut = ens;
	//on se place a droite de l'ensemble qui a crisis
	while(ens->frereD)
	{
		ens = ens->frereD;
	}
	Element * elems = init_element();
	Element * elemtmp=NULL;
	int date = 0;
	elems = ajoute_elemt(elems,crisise->index,crisise->release,crisise->deadline);
	//on backtrack jusqu'a PULL de X ou a la fin
	while(ens)
	{
		
		if(ens->numero_element != -1) // si c'est un element
		{
			elemtmp = get_element_i(touselems,ens->numero_element);
			if(elemtmp->deadline > crisise->deadline)//si on est sur PULL(X)
			{	
				ajoute_elemt(elemspere,elemtmp->index,elemtmp->release,elemtmp->deadline);
				date = ens->temps_depart;
				//printf("PULL(%d) = %d\n",crisise->index,elemtmp->index);
				if(ens2 == NULL)
				{
					ens2 = cree_ensemble(-2,-1);
				}
				else
				{
					tmp = ens2;
					ens2->frereG = cree_ensemble(-2,-1);
					ens2 = ens2->frereG;
					ens2->frereD = tmp;
					
				}
				if(ens->frereG)//on regarde si on est pas au debut de l'ensemble
				{
					ens = ens->frereG;
					libereens(ens->frereD);
					ens->frereD = NULL;
				}
				else
				{
					libereens(ens);
					ens = NULL;
				}
				break;
			}
			if(ens2 == NULL)//initialisation
			{
				ens2 = cree_ensemble(-2,-1);
			}
			else
			{
				tmp = ens2;
				ens2->frereG = cree_ensemble(-2,-1);
				ens2 = ens2->frereG;
				ens2->frereD = tmp;
			}
			
			elems=ajoute_elemt(elems,elemtmp->index,elemtmp->release,elemtmp->deadline);
		}
		
		else //si c'est un ensemble
		{
			if(ens2== NULL)
			{
				ens2 = cpyens(ens);
			}
			else
			{
				ens2->frereG = cpyens(ens);
				ens2->frereG->frereD = ens2;
				ens2 = ens2->frereG;
			}

		}
		
		if(ens->frereG)//on regarde si on est pas au debut de l'ensemble
		{
			
			//printf("On ajoute ");affiche_ensemble(ens);printf("au r.s\n");
			ens = ens->frereG;
			libereens(ens->frereD);
			ens->frereD = NULL;
		}
		else
		{
			//printf("On ajoute ");affiche_ensemble(ens);printf("au r.s\n");
			libereens(ens);
			ens = NULL;
		}
		
		//si ens est null alors on n'a pas trouvé PULL(X) (si on a trouvé pull X on break)
		if(ens == NULL)return NULL;
	}
	
	//affiche_ensemble(ens2);printf(" R.S.\n");
	//affichejobs(elems);
	
	Ensemble* ens3 = NULL;
	int i;
	while(ens2)//parcours de ens2
	{
		if(ens2->numero_element == -2)//si on attend un element
		{
			
			i = eligible(elems,date);
			if(i==-1)
			{
				//printf("Failure 1\n");
				return NULL;
			}
			elemtmp = get_element_i(touselems,i);
			if(elemtmp == NULL)
			{
				//printf("Failure 0\n");
				return NULL;
			}
			
			date = max(date,elemtmp->release);
			if(date+taille_paquet > elemtmp->deadline)//CRISIS
			{
				
				//printf("crisis(crisis) sur la tache %d\n",elemtmp->index);
				Element * crisisrec = ajoute_elemt(NULL,elemtmp->index,elemtmp->release,elemtmp->deadline);
				elems=retire_element_i(elems,crisisrec->index);
				if(!elems)
					elems = ajoute_elemt(elems,INT_MAX,INT_MAX,INT_MAX);
				ens3 = crisis(ens3,crisisrec,elems,touselems,taille_paquet);
				if(ens3 == NULL)
				{
					//printf(" NO PULL(%d) FOUND\n",elemtmp->index);
					return NULL;
				}
				while(ens3->frereD)
					ens3 = ens3->frereD;
				//affiche_ensemble(ensembletmp);printf("Elemenent le plus a droite\n");
				date = date_fin(ens3,taille_paquet);
				
			}
			
			else// PAS DE CRISE
			{
				
				if(ens3 == NULL)//premier element de ens3
				{
					ens3=cree_ensemble(elemtmp->index,date);
				}
				else
				{
					ens3->frereD=cree_ensemble(elemtmp->index,date);
					ens3->frereD->frereG = ens3;
					ens3 = ens3->frereD;
				}
				elems= retire_element_i(elems,elemtmp->index);
				date+=taille_paquet;
				if(ens2->frereD)//Si il y a un prochain element
				{
					if(ens2->frereD->numero_element == -1)//et que c'est un ensemble
					{
						if(date>=ens2->frereD->temps_depart)//ET que ca INVADE
						{
							//printf("%d INVADE date %d\n",elemtmp->index,date);
							
							ens2->frereD->filsG = invade(ens2->frereD->filsG,touselems,date,taille_paquet);
							
							ens2->frereD->temps_depart = ens2->frereD->filsG->temps_depart;
							
							
						}
					}
				}
			}
			
			//affiche_ensemble(ens3);printf(" Element\n");
		}
		else//si c'est un ensemble
		{
			
			if(ens3== NULL)
			{
				
				ens3 =  cree_ensemble(-1,ens2->temps_depart);
				ens3->filsG = cpyens(ens2->filsG);
				
			}
			else
			{
				ens3->frereD = cree_ensemble(-1,ens2->temps_depart);
				ens3->frereD->filsG = cpyens(ens2->filsG);
				ens3->frereD->frereG = ens3;
				ens3 = ens3->frereD;
			}
			//affiche_ensemble(ens2);
			date = date_fin(ens3,taille_paquet);//printf("fin = %d \n",date);

		}
		
		tmp = ens2;
		ens2 = ens2->frereD;
		free(tmp);
	}

	//on remet ens3 a son debut
	//affiche_ensemble(ens);printf(" -----------------------------\n");
	while(ens3->frereG)
		ens3 = ens3->frereG;
	Ensemble * newfils = cree_ensemble(-1,ens3->temps_depart);
	newfils->filsG = ens3; 
	if(!ens)//si ens est vide, alors on est au tout debut 
	{
		//printf("RETOUR DE CRISIS--------------------------------------");
		//affiche_ensemble(newfils);printf("\n");
		return newfils;
	}
	ens->frereD = newfils;
	newfils->frereG = ens;
	//on donne a ens un element avec ens3 comme filsgauche
	//printf("RETOUR DE CRISIS--------------------------------------");
	//affiche_ensemble(debut);printf("\n");
	
	return debut;
	
}


int is_ok(Graphe g, int taille_paquet, int * mi, int * wi)
{
	int nbr_route = g.N /2;
	for(int i=1;i<nbr_route;i++)
	{
		for(int j=i-1;j>=0;j--)
		{
			if( fabs(mi[j]+g.matrice[nbr_route][j]-mi[i]-g.matrice[nbr_route][i]) <taille_paquet )
			{
				printf("PB(aller) entre %d(%d) et %d(%d)\n",i,mi[j]+g.matrice[nbr_route][j],j,mi[i]+g.matrice[nbr_route][i]);
				return 0;
			}
		}
	}

	for(int i=0;i<nbr_route;i++)
	{
		for(int j=0;j<i;j++)
		{
			if( fabs(mi[j]+g.matrice[nbr_route][j]+2*g.matrice[nbr_route][nbr_route+1+j]+wi[j]-wi[i]-mi[i]-g.matrice[nbr_route][i]-2*g.matrice[nbr_route][nbr_route+1+i]) <taille_paquet )
			{
				printf("PB(retour) entre %d(%d) et %d(%d)\n",i,mi[j]+g.matrice[nbr_route][j]+2*g.matrice[nbr_route][nbr_route+1+j],j,mi[i]+g.matrice[nbr_route][i]+2*g.matrice[nbr_route][nbr_route+1+i]);
				return 0;
			}	
		}
	}
	return 1;
}
//Permute les elements d'un tableau
void fisher_yates(int * tab, int taille)
{
	int r;
	int tmp;
	for(int i=taille-1;i>0;i--)
	{
		r=rand()%(i+1);
		tmp = tab[i];
		tab[i]= tab[r];
		tab[r]=tmp;
	}
}
void tri_bulles_inverse(int* tab,int* ordre,int taille)
{
	int sorted;
	int tmp;
	int tmp_ordre;

	int tabcpy[taille];
	for(int i=0;i<taille;i++)tabcpy[i]=tab[i];

	for(int i=taille-1;i>=1;i--)
	{
		sorted = 1;
		for(int j = 0;j<=i-1;j++)
		{

			if(tabcpy[j+1]<tabcpy[j])
			{
				tmp_ordre = ordre[j+1];
				ordre[j+1]=ordre[j];
				ordre[j]=tmp_ordre;
				tmp = tabcpy[j+1];
				tabcpy[j+1]= tabcpy[j];
				tabcpy[j]= tmp;
				sorted = 0;
			}
		}
		if(sorted){return;}
	}

}
//Algo naif
int simons(Graphe g, int taille_paquet, int TMAX, int Periode,int mode)
{
	
	
	 if (!(g.N % 2))
    {
      printf ("G n'est peut être pas une étoile\n");
      exit (5);
    }

	  int nbr_route = g.N / 2;
	  int Dl[nbr_route];
	  int lambdaV[nbr_route];
	  int m_i[nbr_route];
	  int w_i[nbr_route];
	int i;

	int permutation[nbr_route];
	  for (i = 0; i < nbr_route; i++)
	    {
	    	permutation[i]=i;
	      Dl[i] = g.matrice[nbr_route][i]+g.matrice[nbr_route][i+nbr_route+1];
	      lambdaV[i] = g.matrice[nbr_route][i];
	    }

	int a_affecter=nbr_route;

	int offset = 0;
	
	
	if(mode == 0)
	{

		fisher_yates(permutation,nbr_route);
		//affiche_tab(permutation,nbr_route);
		m_i[permutation[0]]=0;
		offset=taille_paquet+g.matrice[nbr_route][permutation[0]];
		for(i=1;i<nbr_route;i++)
		{
			m_i[permutation[i]]=offset-g.matrice[nbr_route][permutation[i]];
			offset+=taille_paquet;
		}
	}
	else
	{
		if(mode == 1)
		{
			//tant qu'on n'a pas affecté toutes les routes
			while(a_affecter != 0)
			{
				i=greater(Dl,nbr_route);
				//Si on est le premier
				if(offset==0)
				{
					m_i[i] = 0;
					offset=taille_paquet+lambdaV[i];
				}
				else
				{
					m_i[i] = offset-lambdaV[i];
					offset += taille_paquet;
				}
				a_affecter--;
				Dl[i]=0;
			}
		}
		else

		{
			//Tri du tableau ordre
			tri_bulles_inverse(Dl,permutation,nbr_route);
			//affiche_tab(Dl,nbr_route);
			//affiche_tab(permutation,nbr_route);
			//On envoie les messages de longest a shortest
			m_i[permutation[0]]=0;
			offset = taille_paquet+g.matrice[nbr_route][permutation[0]];
			for(int i=1;i<nbr_route;i++)
			{
				m_i[permutation[i]]=offset-g.matrice[nbr_route][permutation[i]];
				offset+=taille_paquet;
			}
		}
	}
	

	//release times
	int arrivee[nbr_route];
	offset = 0;
	for(i=0;i<nbr_route;i++)
	{
		Dl[i] = g.matrice[nbr_route][i]+g.matrice[nbr_route][i+nbr_route+1];

		arrivee[i] = Dl[i]+m_i[i]+g.matrice[nbr_route][i+nbr_route+1];
	}
	

	i=lower(arrivee,nbr_route);
	

	int	date=arrivee[i];
	int deadline_fenetre = date + Periode;
	int j;
	//affichetab(arrivee,g.sources);
	//afficheTwoWayTrip(t);
	Element * elems = init_element();
	int deadline_route;

	for(j=0;j<nbr_route;j++)
	{
		deadline_route = TMAX+m_i[j]- g.matrice[nbr_route][j];
		elems = ajoute_elemt(elems,j,arrivee[j],deadline_route);
		//printf(" %d ",deadline_route);

	}

	
	
	//affichejobs(elems);
	//affichejobs(elems);
	Element *  elems2 = cpy_elems(elems);
	Element * tmp = elems2;
	
	int a_scheduler = nbr_route;
	Ensemble * ens = NULL;
	Ensemble * ensembletmp;
	while(a_scheduler != 0)//tant qu'on n'a pas schedul tous les elements
	{
		//printf("test %p\n",elems2);
		i = eligible(elems2,date);
		//printf("i = %d\n",i);
		if(i==-1)
		{
			printf("Failure 1\n");
			return -1;
		}
		tmp = get_element_i(elems2,i);
		
		if(tmp == NULL)
		{
			printf("Failure 0\n");
			return -1;
		}
		
		date = max(date,tmp->release);
		if(date+taille_paquet > tmp->deadline)//CRISIS
		{
			
			printf("Crisis(main) sur la tache %d\n",tmp->index);
			Element * crisise = NULL;
			crisise = ajoute_elemt(crisise,tmp->index,tmp->release,tmp->deadline);
			elems2=retire_element_i(elems2,crisise->index);
			if(!elems2)
					elems2 = ajoute_elemt(elems2,99999,99999,999999);
			//affiche_ensemble(ens);printf(" Avant\n");
			ens = crisis(ens,crisise,elems2,elems,taille_paquet);
			
			//affiche_ensemble(ens);printf(" Apres\n");
			if(ens == NULL)
			{
				//affichejobs(elems2);
				//printf(" NO PULL(%d) FOUND\n",crisise->index);
				return -1;
			}
			//on se remet a droite de l'ensemble
			ensembletmp = ens;
			while(ensembletmp->frereD)
				ensembletmp = ensembletmp->frereD;
			//affiche_ensemble(ensembletmp);printf("Elemenent le plus a droite\n");
			date = date_fin(ens, taille_paquet);
			
			//printf("APRESCRISIS\n");
			//printf("Date %d : ",date);
			//affichejobs(elems2);
			//affiche_ensemble(ens);printf("\n\n\n");
		}
		else // pas crisis
		{
			//printf("AVANT eligible = %d\n",i);
			//printf("Date %d : ",date);
			//affichejobs(elems2);
			//affiche_ensemble(ens);printf("\n");
			if(ens == NULL)//si c'est la premiere fois
			{
				ens = cree_ensemble(tmp->index,date);
				ensembletmp = ens;
			}
			else
			{
				ensembletmp->frereD = cree_ensemble(tmp->index,date);
				ensembletmp->frereD->frereG = ensembletmp;
				ensembletmp = ensembletmp->frereD;
			}
			//printf("APRES\n");
			elems2= retire_element_i(elems2,tmp->index);
			a_scheduler--;
			//printf("Date %d : ",date);
			//affichejobs(elems2);
			//affiche_ensemble(ens);printf("\n\n\n");
			date+=taille_paquet;
			
		}
		
		
	
	}
	//ecriture des temps trouvés
	while(ens)
	{
		w_i[ens->numero_element] = ens->temps_depart-arrivee[ens->numero_element];
		ens=ens->frereD;
	}
	
	libereens(ens);
	freeelems(elems);
	freeelems(elems2);

	//affiche_tab(m_i,nbr_route);
	//affiche_tab(w_i,nbr_route);
	if(!is_ok(g,taille_paquet,m_i,w_i))printf("ERROR\n");
	int max = w_i[0]+2*Dl[0];
	for(int i=0;i<nbr_route;i++)
	{
		if(w_i[i]+2*Dl[i] > max)
			max= w_i[i]+2*Dl[i];
	}
	
	return max;
}
