#ifndef GRAPH_PARSER__H
#define GRAPH_PARSER__H
#include "problem.h"
#include "modal.h"

char * read_line();

void parse_graph(Problem * problem, NodeAxiomGenerator node_gen,
									EdgeAxiomGenerator edge_gen);

void applic_node_axiom_generator(Problem * problem,
								 NodeAxiomGenerator node_gen);

char first_letter_is_numeric(char * str);

void parse_content(Problem * problem, EdgeAxiomGenerator edge_gen);

void parse_edge(Problem * problem, char * line, EdgeAxiomGenerator edge_gen);

void parse_affectation(Problem * problem, char * line);

void parse_negative_affectation(Problem * problem, char * line);

void parse_negative_hypothesis(Problem * problem, char * line);

void parse_negative_knowledge(Problem * problem, char * line);

void parse_positive_affectation(Problem * problem, char * line);

void parse_positive_hypothesis(Problem * problem, char * line);

void parse_positive_knowledge(Problem * problem, char * line);

#endif
