#ifndef MODAL__H
#define MODAL__H
#include "problem.h"

#define PLUS_EDGE 1
#define MINUS_EDGE 0

typedef void (*NodeAxiomGenerator)(Problem * problem, int var); 
typedef void (*EdgeAxiomGenerator)(Problem * problem, int a, int b, char edge_sign); 

int get_hypothesis_index(int var);
int get_neg_hypothesis_index(int var);
int get_knowledge_index(int var);
int get_neg_knowledge_index(int var);

void classic_node_constraint_generator(Problem * problem, int var);

void constraint_knowledge_consistence_axiom(Problem * problem, int var);
void constraint_hypothesis_definition_axiom(Problem * problem, int var);

void constraint_arc_relation_01_axiom(Problem * problem,
									  int a, int b, char edge_sign);
void constraint_arc_relation_02_axiom(Problem * problem,
									  int a, int b, char edge_sign);

void init_modal_problem(Problem * problem, int num_variables, int num_constraints);

void print_modal_solution(Problem * problem, void * user_data);

#endif
