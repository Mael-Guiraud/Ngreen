#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define ROUTE_NUMBER 8
#define MESSAGE_SIZE 2500
#define PERIOD 23000
#define ROUTE_SIZE 20000


 
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

int* genere_reseau(int route_number, int route_size, int period){
  	int *return_time = malloc(sizeof(int)*route_number);
  	for(int i = 0; i<route_number; i++){
    	return_time[i] = (rand()%route_size)%period;
  	}
  	return return_time;
}

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


void search(int message_size, int period, int route_number, int* return_time){
	printf("Instance aléatoire :\n");//we assume that the value in return time are in [0,period[
	for(int i = 0; i < route_number; i++) printf("%d  ",return_time[i]);
	int shift = return_time[0];
	printf("\nInstance aléatoire décalée :\n");
	for (int i = 0; i < route_number; i++){//we shift the values so that the first route has return time 0
		return_time[i] -= shift;
		if (return_time[i] < 0) return_time[i] += period;
		printf("%d  ",return_time[i]);
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
  	
  	/* Call the recursive part with the proper algorithm */
  	if(recursive_search(solution, s, used_route, 1, return_time, route_number, message_size, period)){
  		printf("Solution trouvée \n");
  		print_solution(solution,route_number-1,s);
  	}
  	else{
  		printf("Pas de solution\n");
  	}
  	/* Free the memory */
  	free(solution);
  	free(used_route);
  	for(int i = 0; i < route_number ; i++) {
  		free(s[i].fw_next);
  		free(s[i].bw_next);
  	}
  	free(s);
}




int main(){
  	srand(time(NULL));
  	int *return_time = genere_reseau(ROUTE_NUMBER,ROUTE_SIZE,PERIOD);
  	search(MESSAGE_SIZE,PERIOD,ROUTE_NUMBER,return_time);
  	free(return_time);
  	return 0;
}