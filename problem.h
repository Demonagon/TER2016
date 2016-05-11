#ifndef PROBLEM__H
#define PROBLEM__H

#define TRUE 1
#define FALSE 0
#define UNAFFECTED -1

typedef char (*ConstraintEvaluation)(int * variables,
									 void * data);

typedef struct {
	int num_variables;
	int * variables;
	ConstraintEvaluation evaluation_function;
	void * function_data;
} Constraint;

typedef Constraint * * VariableConstraints;

typedef struct {
	int num_variables;
	int num_constraints;

	int next_constraint;

	Constraint * constraints;

	VariableConstraints * variables_constraints;

	int * values;
} Problem;

typedef void (*ModelFilteringFunction)(Problem * problem, void * user_data); 

VariableConstraints create_variable_constraints(int num_constraints);

/**
* Initialise le problème problem pour un nombre de variables num_variables.
*/
void init_problem(Problem * problem, int num_variables, int num_constraints);

/**
* Libère la mémoire du problème problem.
*/
void free_problem(Problem * problem);

void init_constraint(Constraint * constraint, int num_variables, int * variables,
					 ConstraintEvaluation function, void * data);

void add_constraint_in_variable_constraints(Constraint * constraint,
											VariableConstraints vc,
											int num_constraints);

void add_constraint(Problem * problem, Constraint * constraint);
/**
* Ces fonctions expriment grâce aux couples autorisés différentes opérations
* de la logique du premier ordre.
*/
void constraint_a_and_b(Problem * problem, int a, int b);
void constraint_a_or_b(Problem * problem, int a, int b);
void constraint_not_a_or_not_b(Problem * problem, int a, int b);
void constraint_a_imply_b(Problem * problem, int a, int b);
void constraint_a(Problem * problem, int a);
void constraint_not_a(Problem * problem, int a);

/**

*/
char is_affectation_consistent(Problem * problem, int var);

/**
* Donne la valeur value à la variable var dans le problème problem
*/
void affect_variable(Problem * problem, int var, char value);

void print_solution(Problem * problem, void * user_data);

char backtrack_recursive(Problem * problem, int i,
						 ModelFilteringFunction function,
						 void * user_data);

#endif
