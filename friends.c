#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"
#include "graph.h"

void
add_friend(list_graph_t *graph, char *name1, char *name2)
{
	// obtinem id-ul utilizatorilor
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	// adaugam conexiunea in ambele sensuri
	lg_add_edge(graph, id1, id2, sizeof(uint16_t));
	lg_add_edge(graph, id2, id1, sizeof(uint16_t));

	// afisam mesajul corespunzator
	printf("Added connection %s - %s\n", name1, name2);
}

void
remove_friend(list_graph_t *graph, char *name1, char *name2)
{
	// obtinem id-ul utilizatorilor
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	// stergem conexiunea in ambele sensuri
	lg_remove_edge(graph, id1, id2);
	lg_remove_edge(graph, id2, id1);

	// afisam mesajul corespunzator
	printf("Removed connection %s - %s\n", name1, name2);
}

void
get_distance(list_graph_t *graph, char *name1, char *name2)
{
	// obtinem id-ul utilizatorilor
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	// calculam distanta minima dintre cei doi utilizatori
	int distance = bfs_list_graph(graph, id1, id2);

	if (!distance) {
		// daca nu exista drum intre cei doi utilizatori
		printf("There is no way to get from %s to %s\n", name1, name2);
	} else {
		// exista drum intre cei doi utilizatori, afisam distanta
		printf("The distance between %s - %s is %d\n", name1, name2, distance);
	}
}

void
number_of_friends(list_graph_t *graph, char *name)
{
	// afisam numarul de prieteni ai unui utilizator
	uint16_t id = get_user_id(name);
	printf("%s has %d friends\n", name, graph->neighbors[id]->size);
}

void
most_popular(list_graph_t *graph, char *name)
{
	// obtinem id-ul utilizatorului
	uint16_t id = get_user_id(name);
	// cautam cel mai popular prieten al utilizatorului
	dll_node_t *aux = graph->neighbors[id]->head;

	if (!aux)
		return;

	uint16_t max_friends = 0;
	uint16_t wanted_id = 0;
	for (unsigned int i = 0; i < graph->neighbors[id]->size; i++) {
		uint16_t friend_id = *(uint16_t *)aux->data;
		if (graph->neighbors[friend_id]->size > max_friends) {
			// daca gasim un prieten cu mai multi prieteni decat cei gasiti
			// pana acum, il retinem
			max_friends = graph->neighbors[friend_id]->size;
			wanted_id = friend_id;
		} else if (graph->neighbors[friend_id]->size == max_friends) {
			// in caz de egalitate, retinem utilizatorul cu id-ul minim
			if (friend_id < wanted_id)
				wanted_id = friend_id;
		}
		aux = aux->next;
	}

	// in caz de egalitate, se va prefera pe sine
	if (max_friends <= graph->neighbors[id]->size) {
		printf("%s is the most popular\n", name);
	} else {
		// altfel, unul dintre prietenii sai este cel mai popular
		char *wanted_name = get_user_name(wanted_id);
		printf("%s is the most popular friend of %s\n", wanted_name, name);
	}
}

unsigned int
add_ascending(doubly_linked_list_t *list, uint16_t id)
{
	// pornim de la inceputul listei
	unsigned int pos = 0;
	dll_node_t *aux = list->head;
	// daca lista e goala, singura pozitie disponibila e prima
	if (!list->head) {
		return 0;
	} else if (compare_function_uint16(&id, aux->data) < 0) {
		// daca id-ul dat e mai mic decat id-ul primului nod atunci toate dupa
		// vor fi mai mici, deci singura pozitie disponibila e prima
		return 0;
	}
	while (aux->next && compare_function_uint16(aux->next->data, &id) < 0) {
		// se cauta ultimul id mai mic decat id-ul dat
		aux = aux->next;
		pos++;
	}
	// returnam pozitia imediat urmatoare
	return pos + 1;
}

void
get_common_friends(list_graph_t *graph, char *name1, char *name2)
{
	// obtinem id-urile utilizatorilor
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	// cream o lista pentru a retine prietenii comuni ai acestora
	doubly_linked_list_t *common_friends = dll_create(sizeof(uint16_t));
	// initializam doi pointeri pentru a parcurge listele de prieteni
	dll_node_t *aux_id1 = graph->neighbors[id1]->head;
	dll_node_t *aux_id2 = graph->neighbors[id2]->head;

	// parcurgem lista de vecini ai celui cu mai putini prieteni
	if (graph->neighbors[id1]->size < graph->neighbors[id2]->size) {
		for (unsigned int i = 0; i < graph->neighbors[id1]->size; i++) {
			unsigned int ok = 0;
			// cautam daca in a doua lista exista fiecare utilizator din prima
			while (aux_id2 && ok == 0) {
				if (!compare_function_uint16(aux_id1->data, aux_id2->data)) {
					// daca da, adaugam id-ul in lista de prieteni comuni
					unsigned int pos;
					pos = add_ascending(common_friends,
										*(uint16_t *)aux_id1->data);
					dll_add_nth_node(common_friends, pos, aux_id1->data,
									 sizeof(uint16_t));
					ok = 1;
				}
				aux_id2 = aux_id2->next;
			}
			// resetam pointerul pentru a parcurge din nou lista de vecini
			aux_id2 = graph->neighbors[id2]->head;
			aux_id1 = aux_id1->next;
		}
	} else {
		for (unsigned int i = 0; i < graph->neighbors[id2]->size; i++) {
			unsigned int ok = 0;
			// cautam daca in prima lista exista fiecare utilizator din a doua
			while (aux_id1 && ok == 0) {
				if (!compare_function_uint16(aux_id2->data, aux_id1->data)) {
					// daca da, adaugam id-ul in lista de prieteni comuni
					unsigned int pos;
					pos = add_ascending(common_friends,
										*(uint16_t *)aux_id2->data);
					dll_add_nth_node(common_friends, pos, aux_id2->data,
									 sizeof(uint16_t));
					ok = 1;
				}
				aux_id1 = aux_id1->next;
			}
			// resetam pointerul pentru a parcurge din nou lista de vecini
			aux_id1 = graph->neighbors[id1]->head;
			aux_id2 = aux_id2->next;
		}
	}

	if (!common_friends->size) {
		// nu exista prieteni comuni, adica lista e goala
		printf("No common friends for %s and %s\n", name1, name2);
	} else {
		// afisam prietenii comuni
		printf("The common friends between %s and %s are:\n", name1, name2);
		dll_node_t *aux = common_friends->head;
		while (aux) {
			uint16_t id = *(uint16_t *)aux->data;
			printf("%s\n", get_user_name(id));
			aux = aux->next;
		}
	}
	dll_free(&common_friends);
}

void
get_suggestions(list_graph_t *graph, char *name)
{
	// obtinem id-ul utilizatorului
	uint16_t id = get_user_id(name);

	// cream o lista pentru a retine sugestiile de prieteni
	doubly_linked_list_t *suggested_friends = dll_create(sizeof(uint16_t));
	dll_node_t *aux_id1 = graph->neighbors[id]->head;
	dll_node_t *aux_id2;

	// parcurgem lista de prieteni ai utilizatorului
	for (unsigned int i = 0; i < graph->neighbors[id]->size; i++) {
		aux_id2 = graph->neighbors[*(uint16_t *)aux_id1->data]->head;
		while (aux_id2) {
			// cautam daca prietenii prietenilor sunt si prietenii
			// utilizatorului
			dll_node_t *friend = dll_get_node(graph->neighbors[id],
											  aux_id2->data);
			if (!friend) {
				// daca nu sunt, adaugam id-ul in lista de sugestii
				if (compare_function_uint16(&id, aux_id2->data) != 0) {
					// facem verificarea pentru a nu se adauga pe sine
					dll_node_t *aux = dll_get_node(suggested_friends,
													aux_id2->data);
					if (!aux) {
						unsigned int pos = add_ascending(suggested_friends,
												 *(uint16_t *)aux_id2->data);
						dll_add_nth_node(suggested_friends, pos, aux_id2->data,
										 sizeof(uint16_t));
					}
				}
			}
			aux_id2 = aux_id2->next;
		}
		aux_id1 = aux_id1->next;
	}

	if (!suggested_friends->size) {
		// nu exista sugestii, adica lista e goala
		printf("There are no suggestions for %s\n", name);
	} else {
		// afisam sugestiile
		printf("Suggestions for %s:\n", name);
		dll_node_t *aux = suggested_friends->head;
		while (aux) {
			uint16_t id = *(uint16_t *)aux->data;
			printf("%s\n", get_user_name(id));
			aux = aux->next;
		}
	}
	dll_free(&suggested_friends);
}

void
handle_input_friends(char *input, list_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	// extragem numele utilizatorilor
	char *name1 = strtok(NULL, "\n ");
	char *name2 = strtok(NULL, "\n ");

	if (!strcmp(cmd, "add"))
		add_friend(graph, name1, name2);
	else if (!strcmp(cmd, "remove"))
		remove_friend(graph, name1, name2);
	else if (!strcmp(cmd, "suggestions"))
		get_suggestions(graph, name1);
	else if (!strcmp(cmd, "distance"))
		get_distance(graph, name1, name2);
	else if (!strcmp(cmd, "common"))
		get_common_friends(graph, name1, name2);
	else if (!strcmp(cmd, "friends"))
		number_of_friends(graph, name1);
	else if (!strcmp(cmd, "popular"))
		most_popular(graph, name1);

	free(commands);
}
