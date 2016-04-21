#include <stdio.h>
#include <stdlib.h>
#include "problem.h"

AuthorizedValues create_authorized_values(int num_variables) {
	AuthorizedValues a_values = malloc( num_variables * sizeof(char * *) );

	for(int i = 0; i < num_variables; i++) {
		a_values[i] = malloc( num_variables * sizeof(char *) );
		for(int j = 0; j < num_variables; j++)
			a_values[i][j] = malloc( NUM_POSSIBILITIES * sizeof(char) );
	}

	authorize_all(a_values, num_variables);

	return a_values;
}

void free_authorized_values(AuthorizedValues av, int num_variables) {
	for(int i = 0; i < num_variables; i++) {
		for(int j = 0; j < num_variables; j++) {
			free(av[i][j]);
		}
		free(av[i]);
	}
	free(av);
}

void authorize_all(AuthorizedValues av, int num_variables) {
	for(int i = 0; i < num_variables; i++)
		for(int j = 0; j < num_variables; j++) {
			if( i == j ) continue;
			for(int k = 0; k < NUM_POSSIBILITIES; k++)
				av[i][j][k] = TRUE;
		}
}

void init_problem(Problem * problem, int num_variables) {
	AuthorizedValues autho_values = create_authorized_values(num_variables);
	*problem = (Problem) { 
				 .num_variables = num_variables,
				 .autho_values = autho_values,
				 .values = malloc( num_variables * sizeof(char) ) };
}

void free_problem(Problem * problem) {
	free_authorized_values(problem->autho_values, problem->num_variables);
	free(problem->values);
}

char get_value(char * values, char val_a, char val_b) {
	if( ! val_a ) {
		if( ! val_b ) return values[0];
		else return values[1];
	} else {
		if( ! val_b ) return values[2];
		else return values[3];
	}
}

void set_value(char * values, char val_a, char val_b, char value) {	
	if( ! val_a ) {
		if( ! val_b ) { values[0] = value; return; }
		else { values[1] = value; return; }
	} else {
		if( ! val_b ) { values[2] = value; return; }
		else { values[3] = value; return; }
	}
}

void enable_couple(Problem * problem, int a, int b, char val_a, char val_b) {
	set_value(problem->autho_values[a][b], val_a, val_b, TRUE);
	set_value(problem->autho_values[b][a], val_b, val_a, TRUE);
}

void disable_couple(Problem * problem, int a, int b, char val_a, char val_b) {
	set_value(problem->autho_values[a][b], val_a, val_b, FALSE);
	set_value(problem->autho_values[b][a], val_b, val_a, FALSE);
}

void constraint_a_and_b(Problem * problem, int a, int b) {
	disable_couple(problem, a, b, FALSE, TRUE);
	disable_couple(problem, a, b, TRUE, FALSE);
	disable_couple(problem, a, b, FALSE, FALSE);
}

void constraint_a_or_b(Problem * problem, int a, int b) {
	disable_couple(problem, a, b, FALSE, FALSE);
}

void constraint_not_a_or_not_b(Problem * problem, int a, int b) {
	disable_couple(problem, a, b, TRUE, TRUE);
}

void constraint_a_imply_b(Problem * problem, int a, int b) {
	disable_couple(problem, a, b, TRUE, FALSE);
}

void constraint_a(Problem * problem, int a) {
	for(int var = 0; var < problem->num_variables; var++) {
		if( var == a ) continue;
		disable_couple(problem, a, var, FALSE, FALSE);
		disable_couple(problem, a, var, FALSE, TRUE);
	}
}

void constraint_not_a(Problem * problem, int a) {
	for(int var = 0; var < problem->num_variables; var++) {
		if( var == a ) continue;
		disable_couple(problem, a, var, TRUE, FALSE);
		disable_couple(problem, a, var, TRUE, TRUE);
	}
}

char is_couple_authorized(Problem * problem, int a, int b, 
											   char val_a, char val_b) {
	return get_value(problem->autho_values[a][b], val_a, val_b);
}

char is_affectation_consistent(Problem * problem, int var, char value) {
	for(int other_var = 0; other_var < var; other_var ++)
		if( ! is_couple_authorized(problem,
								   other_var, var,
								   problem->values[other_var],
								   value) ) return FALSE;

	return TRUE;
}

void affect_variable(Problem * problem, int var, char value) {
	problem->values[var] = value;
}

void print_solution(Problem * problem) {
	for(int k = 0; k < problem->num_variables; k++) {
		printf("%d", problem->values[k]);
		if( k != problem->num_variables - 1)
			printf(" ");
	}
	printf("\n");
}

char backtrack_recursive(Problem * problem, int i) {
	char model_exists = FALSE;

	if( i >= problem->num_variables ) {
		print_solution(problem);
		return TRUE;
	}

	if( is_affectation_consistent(problem, i, TRUE) ) {
		affect_variable(problem, i, TRUE);
		if( backtrack_recursive(problem, i + 1) )
			model_exists = TRUE;
	}

	if( is_affectation_consistent(problem, i, FALSE) ) {
		affect_variable(problem, i, FALSE);
		if( backtrack_recursive(problem, i + 1) )
			model_exists = TRUE;
	}

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
