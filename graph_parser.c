#include <stdio.h>
#include <stdlib.h>
#include "graph_parser.h"
#include "modal.h"

char * read_line() {
	static char line[1024];
	if( scanf("%[^\n]\n", line) == EOF )
		return NULL;
	return line;
}

void parse_graph(Problem * problem, NodeAxiomGenerator node_gen,
									EdgeAxiomGenerator edge_gen) {
	char * line = read_line();

	int num_variables, num_constraints;

	sscanf(line, "%d%*[ \t]%d", &num_variables, &num_constraints);

	init_modal_problem(problem, num_variables, num_constraints);

	applic_node_axiom_generator(problem, node_gen);

	parse_content(problem, edge_gen);
}

void applic_node_axiom_generator(Problem * problem, NodeAxiomGenerator node_gen) {
	int num_nodes = problem->num_variables / 4;

	for(int k = 0; k < num_nodes; k++)
		(*node_gen)(problem, k);
}

char first_letter_is_numeric(char * str) {
	return str[0] >= '0' && str[0] <= '9';
}

void parse_content(Problem * problem, EdgeAxiomGenerator edge_gen) {
	char * line;

	while( ( line = read_line() ) != NULL ) {
		if( first_letter_is_numeric(line) )
			parse_edge(problem, line, edge_gen);
		else
			parse_affectation(problem, line);
	}
}

void parse_edge(Problem * problem, char * line, EdgeAxiomGenerator edge_gen) {
	int a, b;
	char sign;

	char a_str[1024];
	char sign_str[1024];
	char b_str[1024];

	sscanf(line, "%[^+- \t]%*[ \t]%[^1234567890]%s", a_str, sign_str, b_str);

	a = atoi(a_str);
	sign = sign_str[0];
	b = atoi(b_str);

	if( sign == '+' )
		(*edge_gen)(problem, a, b, PLUS_EDGE);
	else if( sign == '-' )
		(*edge_gen)(problem, a, b, MINUS_EDGE);
}

void parse_affectation(Problem * problem, char * line) {
	if( line[0] == '-' )
		parse_negative_affectation(problem, line + 1);
	else
		parse_positive_affectation(problem, line);
}

void parse_negative_affectation(Problem * problem, char * line) {
	if( line[0] == 'H' )
		parse_negative_hypothesis(problem, line + 1);
	else
		parse_negative_knowledge(problem, line);
}

void parse_negative_hypothesis(Problem * problem, char * line) {
	if( line[0] == '-' ) {
		int value;
		sscanf(line + 1, "%d", &value);

		constraint_not_a(problem, get_neg_hypothesis_index(value) );
	}
	else {
		int value;
		sscanf(line, "%d", &value);

		constraint_not_a(problem, get_hypothesis_index(value) );
	}
}

void parse_negative_knowledge(Problem * problem, char * line) {
	if( line[0] == '-' ) {
		int value;
		sscanf(line + 1, "%d", &value);

		constraint_not_a(problem, get_neg_knowledge_index(value) );
	}
	else {
		int value;
		sscanf(line, "%d", &value);

		constraint_not_a(problem, get_knowledge_index(value) );
	}
}

void parse_positive_affectation(Problem * problem, char * line) {
	if( line[0] == 'H' )
		parse_positive_hypothesis(problem, line + 1);
	else
		parse_positive_knowledge(problem, line);
}

void parse_positive_hypothesis(Problem * problem, char * line) {
	if( line[0] == '-' ) {
		int value;
		sscanf(line + 1, "%d", &value);

		constraint_a(problem, get_neg_hypothesis_index(value) );
	}
	else {
		int value;
		sscanf(line, "%d", &value);

		constraint_a(problem, get_hypothesis_index(value) );
	}
}

void parse_positive_knowledge(Problem * problem, char * line) {
	if( line[0] == '-' ) {
		int value;
		sscanf(line + 1, "%d", &value);

		constraint_a(problem, get_neg_knowledge_index(value) );
	}
	else {
		int value;
		sscanf(line, "%d", &value);

		constraint_a(problem, get_knowledge_index(value) );
	}
}

int main_test_01(int argc, char ** argv) {
	/*Problem problem;
	init_problem(&problem, 4);

	disable_couple(&problem, 0, 1, TRUE, FALSE);
	disable_couple(&problem, 1, 2, TRUE, FALSE);
	disable_couple(&problem, 2, 3, TRUE, FALSE);
	disable_couple(&problem, 3, 0, TRUE, FALSE);
	
	char model_exists = backtrack_recursive(&problem, 0, print_solution);

	printf("model existe ? = %d\n", model_exists);

	free_problem(&problem);	*/
	
	/*
	affect_variable(&problem, 0, TRUE);
	affect_variable(&problem, 1, TRUE);
	affect_variable(&problem, 2, TRUE);

	printf("possible v : %d\n", is_affectation_consistent(&problem, 3, TRUE));
	printf("possible f : %d\n", is_affectation_consistent(&problem, 3, FALSE));
	*/

	return 0;
}

int main_test_modal_01(int argc, char ** argv) {
	Problem problem;
	init_modal_problem(&problem, 3, 9);

	constraint_knowledge_consistence_axiom(&problem, 0);
	constraint_hypothesis_definition_axiom(&problem, 0);
	constraint_knowledge_consistence_axiom(&problem, 1);
	constraint_hypothesis_definition_axiom(&problem, 1);
	constraint_knowledge_consistence_axiom(&problem, 2);
	constraint_hypothesis_definition_axiom(&problem, 2);

	constraint_arc_relation_01_axiom(&problem, 0, 1, MINUS_EDGE);
	constraint_arc_relation_01_axiom(&problem, 1, 2, MINUS_EDGE);
	constraint_arc_relation_01_axiom(&problem, 2, 0, PLUS_EDGE);

	backtrack_recursive(&problem, 0, print_modal_solution);

	free_problem(&problem);

	return 0;
}

int main_test_graph_01(int argc, char ** argv) {
	Problem problem;
	
	parse_graph(&problem, classic_node_constraint_generator,
						  constraint_arc_relation_01_axiom);

	backtrack_recursive(&problem, 0, print_modal_solution);

	free_problem(&problem);

	return 0;
}

int main(int argc, char ** argv) {
	return main_test_graph_01(argc, argv);
}
