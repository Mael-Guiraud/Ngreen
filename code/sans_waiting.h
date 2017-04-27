

//retourne -1 si la periode donnée ne donne pas de solutions, la taille de la periode sinon
int algo_shortest_longest(Graphe g,int periode,int taille_message);

//retourne -1 si la periode donnée ne donne pas de solutions, la taille de la periode sinon
int algo_3NT(Graphe g,int periode,int taille_message);


//retourne -1 si la periode donnée ne donne pas de solutions, la taille de la periode sinon
int algo_prime(Graphe g,int periode,int taille_message);


//Recherche linéaires de la plus petite taille pour chaque algo
int linear_3NT(Graphe g,int taille_message);

int linear_prime(Graphe g,int taille_message);

int linear_brute(Graphe g,int taille_message);