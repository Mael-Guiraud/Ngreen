
int simons(Graphe g, int taille_paquet, int TMAX,int periode, int mode_test, int mode);
void affiche_solution(Graphe g, int taille_paquet, int * mi, int * wi);
int is_ok(Graphe g, int taille_paquet, int * mi, int * wi, int p);
void fisher_yates(int * tab, int taille);
void tri_bulles_inverse(int* tab,int* ordre,int taille);
int greater(int * tab,int taille);
int lower(int * tab,int taille);
void tri_bulles(int* tab,int* ordre,int taille);
int simons_periodique(Graphe g, int taille_paquet,int TMAX, int periode, int mode);



void affiche_etoile(Graphe g);