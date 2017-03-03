
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

Ensemble * init_ensemble();
Element * init_element();
void affiche_ensemble_unique(Ensemble * ens);
void affiche_ensemble(Ensemble * ens);
Element * ajoute_elemt(Element * elem,int index, int release, int deadline);
Ensemble * cree_ensemble(int numero, int temps);
Element* retire_element_i(Element * elem, int index);
Element * get_element_i(Element * elem, int index);
void freeelems(Element * elem);
void affichejobs(Element* elem);
Element * cpy_elems(Element * elems);
int date_fin(Ensemble * ens);
int eligible(Element * elems, int time);
void libereens(Ensemble * ens);
Ensemble * cpyens(Ensemble * ens);
Ensemble * invade(Ensemble * ens,Element * touselems,int depart);
Ensemble * crisis(Ensemble * ens,Element * crisise, Element * elemspere,Element * touselems);
int* simons(Graphe g);


