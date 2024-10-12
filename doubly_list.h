
#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define DIE(assertion, call_description)                       \
	do {                                                       \
		if (assertion) {                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);                          \
			exit(errno);                                       \
		}                                                      \
	} while (0)

#define MAX_NAME_SIZE 100

typedef struct dll_node_t {
	// Pentru ca datele stocate sa poata avea orice tip, folosim
	// un pointer la void.
	void *data;
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t {
	dll_node_t *head, *tail;
	unsigned int data_size;
	unsigned int size;
} doubly_linked_list_t;

/**
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii list).
 */
doubly_linked_list_t*
dll_create(unsigned int data_size);

/**
 * Functia returneaza un pointer la nodul ce contine id-ul trimis ca parametru.
 */
dll_node_t *
dll_get_node(doubly_linked_list_t *list, void *id);

/**
 * Functia adauga un nod in lista pe pozitia n.
 */
void
dll_add_nth_node(doubly_linked_list_t *list, unsigned int n, void *data,
				 unsigned int data_size);

/**
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Functia intoarce un pointer spre acest nod proaspat eliminat din
 * lista.
 */
dll_node_t*
dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);

/**
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit elibereaza memoria folosita de structura lista.
 */
void
dll_free(doubly_linked_list_t **pp_list);

/**
 * Functie care compara doua elemente de tip uint16_t.
 */
int
compare_function_uint16(void *a, void *b);

#endif
