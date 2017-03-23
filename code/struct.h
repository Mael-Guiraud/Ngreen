
typedef struct graphe{
	int N; //nombre de sommets
	int ** matrice;//matrice d'adjacence pondérée
}Graphe ;

typedef struct couples{
	int source;
	int target;
}Couples;

typedef struct routage_graph{
	Graphe g;
	int nombre_routes;
	Couples * couples;
}Routage_Graph;