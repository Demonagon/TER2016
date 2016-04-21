#ifndef MODAL__H
#define MODAL__H
#include "problem.h"

#define PLUS_EDGE 1
#define MINUS_EDGE 0

int get_hypothesis_index(int var);
int get_negHypothesis_index(int var);
int get_knowlegde_index(int var);
int get_negKnowlegde_index(int var);

void constraint_knowlegde_consistence_axiom(Problem * problem, int var);
void constraint_hypothesis_definition_axiom(Problem * problem, int var);

void constraint_arc_relation_01_axiom(Problem * problem,
									  int a, int b, char edge_sign);

void init_modal_problem(Problem * problem, int num_variables);

void print_modal_solution(Problem * problem);

#endif
