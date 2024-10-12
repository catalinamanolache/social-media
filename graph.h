#ifndef GRAPH_H
#define GRAPH_H

#include "doubly_list.h"

typedef struct list_graph_t {
	// Listele de adiacenta ale grafului.
	doubly_linked_list_t **neighbors;
	// Numarul de noduri din graf.
	int nodes;
} list_graph_t;

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru lista de adiacenta a grafului.
 */
list_graph_t*
lg_create(int nodes, unsigned int data_size);

/**
 * Adauga o muchie intre nodurile primite ca parametri.
 */
void
lg_add_edge(list_graph_t *graph, int src, int dest, unsigned int data_size);

/**
 * Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar.
 */
int
lg_has_edge(list_graph_t *graph, int src, int dest);

/**
 * Elimina muchia dintre nodurile primite ca parametri.
 */
void
lg_remove_edge(list_graph_t *graph, int src, int dest);

/**
 * Elibereaza memoria folosita de lista de adiacenta a grafului.
 */
void
lg_free(list_graph_t **graph);

/**
 * Functia returneaza distanta minima dintre doua noduri.
 */
int
bfs_list_graph(list_graph_t *lg, uint16_t node1, uint16_t node2);

#endif
