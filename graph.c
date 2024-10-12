
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include "graph.h"
#include "doubly_list.h"
#include "queue.h"
#include "friends.h"

list_graph_t*
lg_create(int nodes, unsigned int data_size)
{
	list_graph_t *list = malloc(sizeof(list_graph_t));
	DIE(!list, "malloc failed");

	list->nodes = nodes;
	list->neighbors = malloc(nodes * sizeof(doubly_linked_list_t *));
	DIE(!list->neighbors, "malloc failed");

	for (int i = 0; i < nodes; i++)
		list->neighbors[i] = dll_create(data_size);
	return list;
}

void
lg_add_edge(list_graph_t *graph, int src, int dest, unsigned int data_size)
{
	dll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size,
					 &dest, data_size);
}

int
lg_has_edge(list_graph_t *graph, int src, int dest)
{
	dll_node_t *aux = graph->neighbors[src]->head;
	while (aux) {
		if (!compare_function_uint16(aux->data, &dest))
			return 1;
		aux = aux->next;
	}
	return 0;
}

void
lg_remove_edge(list_graph_t *graph, int src, int dest)
{
	dll_node_t *aux = graph->neighbors[src]->head;
	dll_node_t *torem;
	int n = 0;
	while (aux) {
		if (!compare_function_uint16(aux->data, &dest))
			torem = dll_remove_nth_node(graph->neighbors[src], n);
		n++;
		aux = aux->next;
	}
	free(torem->data);
	free(torem);
}

void
lg_free(list_graph_t **graph)
{
	for (int i = 0; i < (*graph)->nodes; i++)
		dll_free(&(*graph)->neighbors[i]);
	free((*graph)->neighbors);
	free(*graph);
}

int
bfs_list_graph(list_graph_t *lg, uint16_t node1, uint16_t node2)
{
	uint16_t *visited = calloc(lg->nodes, sizeof(uint16_t));
	uint16_t *distance = calloc(lg->nodes, sizeof(uint16_t));
	queue_t *q = q_create(sizeof(uint16_t), lg->nodes);

	visited[node1] = 1;
	q_enqueue(q, &node1);

	while (q_is_empty(q) != 1) {
		uint16_t x = *(uint16_t *)q_front(q);
		q_dequeue(q);

		dll_node_t *aux = lg->neighbors[x]->head;
		for (unsigned int i = 0; i < lg->neighbors[x]->size; i++) {
			if (visited[*(uint16_t *)aux->data] == 0) {
				visited[*(uint16_t *)aux->data] = 1;
				distance[*(uint16_t *)aux->data] = distance[x] + 1;
				q_enqueue(q, aux->data);
			}
			aux = aux->next;
		}
	}

	int dist = distance[node2];
	free(visited);
	free(distance);
	q_free(q);
	return dist;
}
