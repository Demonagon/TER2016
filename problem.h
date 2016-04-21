#ifndef PROBLEM__H
#define PROBLEM__H

/* 4 car il y a 4 possibilités de couples : 00, 01, 10 et 11 */
#define NUM_POSSIBILITIES 4
#define TRUE 1
#define FALSE 0

typedef char * * * AuthorizedValues;

typedef struct {
	int num_variables;
	AuthorizedValues autho_values;
	char * values;
} Problem;

/**
* Alloue et renvoie un tableau AuthorizedValues à trois dimensions, de taille
* num_variables * num_variables * NUM_POSSIBILITIES
*/
AuthorizedValues create_authorized_values(int num_variables);

/**
* Désalloue le tableau créé par la fonction au dessus
*/
void free_authorized_values(AuthorizedValues av, int num_variables);

/**
* Autorise toute les valeurs de tout les couples. Est utilisé notamment à
* l'initialisation
*/
void authorize_all(AuthorizedValues av, int num_variables);

/**
* Initialise le problème problem pour un nombre de variables num_variables.
*/
void init_problem(Problem * problem, int num_variables);

/**
* Libère la mémoire du problème problem.
*/
void free_problem(Problem * problem);

/**
* Renvoie la case correspondante au couple de valeurs (val_a, val_b) dans le
* tableau values, selont le schéma suivant :
* values[0] <=> (0, 0)
* values[1] <=> (0, 1)
* values[2] <=> (1, 0)
* values[3] <=> (1, 1)
*/
char get_value(char * values, char val_a, char val_b);

/**
* Affecte à value la case de values donnée par un protocole identique à la
* fonction du dessus.
*/
void set_value(char * values, char val_a, char val_b, char value);

/**
* Autorise le couple a, b à posséder la valeur val_a, val_b dans problem.
*/
void enable_couple(Problem * problem, int a, int b, char val_a, char val_b);

/**
* Interdit le couple a, b à posséder la valeur val_a, val_b dans problem.
*/
void disable_couple(Problem * problem, int a, int b, char val_a, char val_b);

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
* Renvoie si oui ou non le couple a, b peut être affecté aux valeurs val_a,
* val_b
*/
char is_couple_authorized(Problem * problem, int a, int b, 
											   char val_a, char val_b);

/**
* Renvoie si l'affectation de var à value est autorisé ; ie si tout les couples
* que forme var avec les variables déjà affectées autorisent cette valeur.
* Important : cette fonction assume que les variables sont affectées dans
* l'ordre croissant.
*/
char is_affectation_consistent(Problem * problem, int var, char value);

/**
* Donne la valeur value à la variable var dans le problème problem
*/
void affect_variable(Problem * problem, int var, char value);

char backtrack_recursive(Problem * problem, int i);

#endif
