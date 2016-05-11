#include "extension.h"
#include "modal.h"
#include "graph_parser.h"
#include <stdlib.h>
#include <stdio.h>

int * get_extension(int k, Extensions * e) {
	if( k >= e->num_extensions ) {
		fprintf(stderr, "Error : no extension n°%d (%d extensions)\n",
				k, e->num_extensions);
		exit(-1);
	}
	return e->extensions[k];
}

void set_extension(int k, Extensions * e, int * extension) {
	if( k >= e->array_size ) {
		fprintf(stderr, "Error : extension n°%d not allowed (array size %d)\n",
				k, e->array_size);
		exit(-1);
	}
	e->extensions[k] = extension;
}

void copy_extension(int k, Extensions * e, int * extension) {
	int * new_extension = malloc( sizeof(int) * e->extension_size );

	for(int i = 0; i < e->extension_size; i++)
		new_extension[i] = extension[i];

	set_extension(k, e, new_extension);
}

void init_extensions(Extensions * e, int increment_size, int extension_size) {
	e->increment_size = increment_size;
	e->extensions = malloc( sizeof(int *) * increment_size);
	e->num_extensions = 0;
	e->array_size = increment_size;
	e->extension_size = extension_size;
}

void free_extensions(Extensions * e) {
	for(int k = 0; k < e->array_size; k++)
		free(e->extensions[k]);
	free(e->extensions);
}

void extend_extensions(Extensions * e) {
	int new_size = e->array_size + e->increment_size;
	int * * new_array = malloc( sizeof(int *) * new_size );

	for(int k = 0; k < e->num_extensions; k++)
		new_array[k] = e->extensions[k];

	free(e->extensions);

	e->extensions = new_array;

	e->array_size = new_size;
}

void add_extension(Extensions * e, int * extension) {
	while( e->num_extensions >= e->array_size )
		extend_extensions(e);

	copy_extension(e->num_extensions, e, extension);
	e->num_extensions++;
}

void remove_extension(Extensions * e, int k) {
	free(e->extensions[k]);
	e->extensions[k] = e->extensions[e->num_extensions - 1];
	e->num_extensions--;
}

int is_subextension(int * extension_a, int * extension_b, int size) {
	for(int k = 0; k < size/4; k ++) {
		int h_index = get_hypothesis_index(k);
		int neg_h_index = get_neg_hypothesis_index(k);

		if( extension_a[h_index] )
			if( ! extension_b[h_index] ) return FALSE;
		if( extension_a[neg_h_index] )
			if( ! extension_b[neg_h_index] ) return FALSE;
	}

	return TRUE;
}

void submit_extension(Extensions * e, int * extension) {

	for(int k = 0; k < e->num_extensions; k++)
		if( is_subextension(extension, e->extensions[k], e->extension_size) ) {
			return;
		}

	for(int k = 0; k < e->num_extensions; k++)
		if( is_subextension(e->extensions[k], extension, e->extension_size) ) {
			remove_extension(e, k);
			k--;
		}

	add_extension(e, extension);
}

void print_extensions(Extensions * e) {
	printf("Extensions :\n");

	for(int i = 0; i < e->num_extensions; i++) {
		int * extension = e->extensions[i];
		int doOnce = TRUE;
		printf("[");

		for(int j = 0; j < e->extension_size; j++) {
			if( ! extension[j] ) continue;

			if( doOnce )
				doOnce = FALSE;
			else
				printf(", ");

			//if( ! extension[j] ) printf("-");

			switch(j % 4) {
				case 0 :
					printf("H%d", j/4); break;
				case 1 :
					printf("H-%d", j/4); break;
				case 2 :
					printf("L%d", j/4); break;
				case 3 :
					printf("L-%d", j/4); break;
			}
		}

		printf("]\n");
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

	backtrack_recursive(&problem, 0, print_modal_solution, NULL);

	free_problem(&problem);

	return 0;
}

int main_test_graph_01(int argc, char ** argv) {
	Problem problem;
	
	parse_graph(&problem, classic_node_constraint_generator,
						  constraint_arc_relation_01_axiom);

	backtrack_recursive(&problem, 0, print_modal_solution, NULL);

	free_problem(&problem);

	return 0;
}

void filter_extensions(Problem * problem, void * user_data) {
	Extensions * extensions = user_data;

	submit_extension(extensions, problem->values);
} 

int main_test_extensions_01(int argc, char ** argv) {
	Problem problem;
	
	parse_graph(&problem, classic_node_constraint_generator,
						  constraint_arc_relation_01_axiom);

	Extensions extensions;
	init_extensions(&extensions, 10, problem.num_variables);

	backtrack_recursive(&problem, 0, filter_extensions, &extensions);

	print_extensions(&extensions);

	free_extensions(&extensions);

	free_problem(&problem);

	return 0;
}

int main(int argc, char ** argv) {
	return main_test_extensions_01(argc, argv);
}
