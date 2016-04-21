CC = gcc

INCLUDE = -I/usr/local/include -I/usr/include
LIB = -L/usr/local/lib -L/usr/lib -g -Wall -std=c99

all: problem

problem: problem.c problem.h modal.c modal.h graph_parser.c graph_parser.h
	$(CC) -o problem $(INCLUDE) problem.c problem.h modal.c modal.h graph_parser.c graph_parser.h $(LIB)
clear:
	-rm *.o *~
	-rm problem
