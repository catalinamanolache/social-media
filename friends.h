#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

#include "graph.h"

/**
 * Functie care creeaza reteaua de prietenie.
 */
list_graph_t *
create_network(void);

/**
 * Functie care creeaza o legatura intre doi utilizatori.
 */
void
add_friend(list_graph_t *graph, char *name1, char *name2);

/**
 * Functie care sterge o legatura intre doi utilizatori.
 */
void
remove_friend(list_graph_t *graph, char *name1, char *name2);

/**
 * Functie care calculeaza distanta minima dintre doi utilizatori.
 */
void
get_distance(list_graph_t *graph, char *name1, char *name2);

/**
 * Functie care calculeaza numarul de prieteni ai unui utilizator.
 */
void
number_of_friends(list_graph_t *graph, char *name);

/**
 * Functie care afiseaza cel mai popular prieten al unui utilizator.
 */
void
most_popular(list_graph_t *graph, char *name);

/**
 * Functie care adauga un element intr-o lista in ordine crescatoare.
 */
unsigned int
add_ascending(doubly_linked_list_t *list, uint16_t id);

/**
 * Functie care afiseaza prietenii comuni ai doi utilizatori.
 */
void
get_common_friends(list_graph_t *graph, char *name1, char *name2);

/**
 * Functie care afiseaza sugestii de prieteni pentru un utilizator.
 */
void
get_suggestions(list_graph_t *graph, char *name);

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *graph);

#endif // FRIENDS_H
