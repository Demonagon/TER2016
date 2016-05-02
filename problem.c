#include <stdio.h>
#include <stdlib.h>
#include "problem.h"

VariableConstraints create_variable_constraints(int num_constraints) {
	VariableConstraints c = malloc( sizeof( Constraint * ) * num_constraints );
	for(int k = 0; k < num_constraints; k++)
		c[k] = NULL;

	return c;
}

void init_problem(Problem * problem, int num_variables, int num_constraints) {
	problem->num_variables = num_variables;
	problem->num_constraints = num_constraints;

	problem->next_constraint = 0;

	problem->constraints = malloc( sizeof(Constraint) * num_constraints );

	problem->variables_constraints = malloc( sizeof(VariableConstraints)
											 * num_variables );

	for(int k = 0; k < num_variables; k++)
		problem->variables_constraints[k] =
			create_variable_constraints(num_constraints);

	problem->values = malloc( sizeof(int) * num_variables);

	for(int k = 0; k < num_variables; k++)
		problem->values[k] = -1;
}

void free_problem(Problem * problem) {
	for(int k = 0; k < problem->num_variables; k++)
		free(problem->variables_constraints[k]);

	free(problem->variables_constraints);

	free(problem->constraints);

	free(problem->values);
}

void init_constraint(Constraint * constraint, int num_variables, int * variables,
					 ConstraintEvaluation function, void * data) {
	constraint->num_variables = num_variables;
	constraint->variables = malloc( sizeof(int) * num_variables );
	for(int k = 0; k < num_variables; k++)
		constraint->variables[k] = variables[k];

	constraint->evaluation_function = function;
	constraint->function_data = data;
}

void add_constraint_in_variable_constraints(Constraint * constraint,
											VariableConstraints vc,
											int num_constraints) {
	for(int k = 0; k < num_constraints; k++) {
		if( vc[k] != NULL ) continue;
		vc[k] = constraint;
		return;
	}
}

void add_constraint(Problem * problem, Constraint * constraint) {
	if( problem->next_constraint >= problem->num_constraints ) {
		fprintf(stderr, "Error : too much constraints for initial declared number\n");
		printf("%d\n", problem->num_constraints);
		exit(-1);
	}

	problem->constraints[problem->next_constraint] = *constraint;

	for(int var_index = 0; var_index < constraint->num_variables; var_index++) {
		int var = constraint->variables[var_index];
		add_constraint_in_variable_constraints(
			problem->constraints + problem->next_constraint,
			problem->variables_constraints[var],
			problem->num_constraints);
	}

	problem->next_constraint++;
}

void add_binary_constraint(Problem * problem, int a, int b,
						   ConstraintEvaluation function) {
	Constraint constraint;

	int variables[] = {a, b};

	init_constraint(&constraint, 2, variables,
					function, problem);

	add_constraint(problem, &constraint);
}

char binary_and_constraint_evaluation(int * variables,
							   		  void * data) {
	Problem * problem = data;
	int a = problem->values[variables[0]];
	int b = problem->values[variables[1]];

	if( a == -1 || b == -1 ) return TRUE;

	return ( a == 1 && b == 1 ); 
}

void constraint_a_and_b(Problem * problem, int a, int b) {
	add_binary_constraint(problem, a, b, binary_and_constraint_evaluation);
}

char binary_or_constraint_evaluation(int * variables,
							   		 void * data) {
	Problem * problem = data;
	int a = problem->values[variables[0]];
	int b = problem->values[variables[1]];

	if( a == -1 || b == -1 ) return TRUE;

	return ( a == 1 || b == 1 ); 
}

void constraint_a_or_b(Problem * problem, int a, int b) {
	add_binary_constraint(problem, a, b, binary_or_constraint_evaluation);
}

char binary_not_or_not_constraint_evaluation(
									int * variables,
							   		void * data) {
	Problem * problem = data;
	int a = problem->values[variables[0]];
	int b = problem->values[variables[1]];

	if( a == -1 || b == -1 ) return TRUE;

	return ( a == 0 || b == 0 ); 
}

void constraint_not_a_or_not_b(Problem * problem, int a, int b) {
	add_binary_constraint(problem, a, b, binary_not_or_not_constraint_evaluation);
}

char binary_imply_constraint_evaluation(
							   		int * variables,
							   		void * data) {
	Problem * problem = data;
	int a = problem->values[variables[0]];
	int b = problem->values[variables[1]];

	if( a == -1 || b == -1 ) return TRUE;

	return ( a == 0 || b == 1 ); 
}

void constraint_a_imply_b(Problem * problem, int a, int b) {
	add_binary_constraint(problem, a, b, binary_imply_constraint_evaluation);
}

void add_unary_constraint(Problem * problem, int a,
						   ConstraintEvaluation function) {
	Constraint constraint;

	int variables[] = {a};

	init_constraint(&constraint, 1, variables,
					function, problem);

	add_constraint(problem, &constraint);
}

char unary_a_constraint_evaluation( int * variables,
							   		void * data) {
	Problem * problem = data;
	int a = problem->values[variables[0]];

	return a; 
}

void constraint_a(Problem * problem, int a) {
	add_unary_constraint(problem, a, unary_a_constraint_evaluation);
}

char unary_not_constraint_evaluation( int * variables,
							   		  void * data) {
	Problem * problem = data;
	int a = problem->values[variables[0]];

	return a == -1 ? TRUE : !a; 
}

void constraint_not_a(Problem * problem, int a) {
	add_unary_constraint(problem, a, unary_not_constraint_evaluation);
}

char is_affectation_consistent(Problem * problem, int var) {
	VariableConstraints constraints = problem->variables_constraints[var];

	for(int k = 0; k < problem->num_constraints; k++) {
		
		Constraint * constraint = constraints[k];

		if( constraint == NULL ) return TRUE;

		if( ! (*constraint->evaluation_function)(constraint->variables,
											     constraint->function_data) )
			return FALSE;		
	}

	return TRUE;

}

void affect_variable(Problem * problem, int var, char value) {
	problem->values[var] = value;
}

void print_solution(Problem * problem) {
	printf("Model : [");
	for(int k = 0; k < problem->num_variables; k++) {
		printf("%d", problem->values[k]);
		if( k != problem->num_variables - 1)
			printf(", ");
	}
	printf("]\n");
}

char backtrack_recursive(Problem * problem, int i,
						 ModelFilteringFunction function) {
	char model_exists = FALSE;

	if( i >= problem->num_variables ) {
		(*function)(problem);
		affect_variable(problem, i, UNAFFECTED);
		return TRUE;
	}

	affect_variable(problem, i, TRUE);
	if( is_affectation_consistent(problem, i) ) {
		if( backtrack_recursive(problem, i + 1, function) )
			model_exists = TRUE;
	}

	affect_variable(problem, i, FALSE);
	if( is_affectation_consistent(problem, i) ) {
		if( backtrack_recursive(problem, i + 1, function) )
			model_exists = TRUE;
	}
	
	affect_variable(problem, i, UNAFFECTED);

	return model_exists;
}

/*
-> i = 0

-> backtrack(i)
-> modele_existe = false
-> si i > taille
	-> afficher modèle
	-> returner vrai
-> affecte 1 à la variable i
	-> si non consistant returner faux
	-> sinon si backtract(i + 1)
		modele_existe = true
-> affecte 0 à la variable i
	-> si non consistant returner faux
	-> sinon si backtract(i + 1)
		modele_existe = true
-> retourner modele_existe
*/
