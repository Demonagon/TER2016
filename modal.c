#include "modal.h"
#include "problem.h"
#include <stdio.h>
#include <stdlib.h>

int get_hypothesis_index(int var) {
	return var * 4 + 0;
}

int get_neg_hypothesis_index(int var) {
	return var * 4 + 1;
}

int get_knowledge_index(int var) {
	return var * 4 + 2;
}

int get_neg_knowledge_index(int var) {
	return var * 4 + 3;
}

void classic_node_constraint_generator(Problem * problem, int var) {
	constraint_knowledge_consistence_axiom(problem, var);
	constraint_hypothesis_definition_axiom(problem, var);
}

void constraint_knowledge_consistence_axiom(Problem * problem, int var) {
	constraint_not_a_or_not_b(problem,
							  get_knowledge_index(var),
							  get_neg_knowledge_index(var));
}

void constraint_hypothesis_definition_axiom(Problem * problem, int var) {
	constraint_not_a_or_not_b(problem,
							  get_hypothesis_index(var),
							  get_neg_knowledge_index(var));
	constraint_not_a_or_not_b(problem,
							  get_neg_hypothesis_index(var),
							  get_knowledge_index(var));
								
}

void constraint_arc_relation_01_axiom(Problem * problem,
									  int a, int b, char edge_sign) {
	if(edge_sign == PLUS_EDGE) {
		constraint_a_imply_b(problem,
							 get_hypothesis_index(a),
							 get_knowledge_index(b));
		constraint_a_imply_b(problem,
							 get_neg_hypothesis_index(a),
							 get_neg_knowledge_index(b));
	} else {
		constraint_a_imply_b(problem,
							 get_hypothesis_index(a),
							 get_neg_knowledge_index(b));
		constraint_a_imply_b(problem,
							 get_neg_hypothesis_index(a),
							 get_knowledge_index(b));
	}
}

void constraint_arc_relation_02_axiom(Problem * problem,
									  int a, int b, char edge_sign) {
	//TODO logique des défauts
}

void init_modal_problem(Problem * problem, int num_variables) {
	init_problem(problem, num_variables * 4);
}

void print_modal_solution(Problem * problem) {
	if( problem->num_variables % 4 ) {
		fprintf(stderr, "Error : Modal problem must have 4n variables (%d)\n",
				problem->num_variables);
		exit(-1);
	}

	for(int k = 0; k < problem->num_variables; k+=4)
		if( ! problem->values[k + 0] && ! problem->values[k + 1] ) return;

	printf("Model : [");

	char doOnce = TRUE;

	for(int k = 0; k < problem->num_variables; k++) {
		if( ! problem->values[k] ) continue;

		if( doOnce )
			doOnce = FALSE;
		else
			printf(", ");

		switch(k % 4) {
			case 0 :
				printf("H%d", k/4); break;
			case 1 :
				printf("H-%d", k/4); break;
			case 2 :
				printf("L%d", k/4); break;
			case 3 :
				printf("L-%d", k/4); break;
		}
	}

	printf("]\n");
}
