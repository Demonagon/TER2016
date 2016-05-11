#ifndef EXTENSION_H
#define EXTENSION_H

typedef struct {
	int * * extensions;
	int num_extensions;
	int array_size;
	int extension_size;
	int increment_size;
} Extensions;

int * get_extension(int k, Extensions * e);
void set_extension(int k, Extensions * e, int * extension);
void copy_extension(int k, Extensions * e, int * extension);

void init_extensions(Extensions * e, int increment_size, int extension_size);
void free_extensions(Extensions * e);

void extend_extensions(Extensions * e);

void add_extension(Extensions * e, int * extension);

void remove_extension(Extensions * e, int k);

// Renvoie TRUE si a est inclu dans b.
int is_subextension(int * extension_a, int * extension_b, int size);

void submit_extension(Extensions * e, int * extension);

void print_extensions(Extensions * e);

#endif
