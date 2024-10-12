#include "feed.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "posts.h"
#include "users.h"

void
feed(list_graph_t *graph, post_array_t *posts, char *name,
	 unsigned int feed_size)
{
	// obtinem id-ul utilizatorului
	uint16_t id = get_user_id(name);
	dll_node_t *aux;

	// daca numarul de postari cerut este mai mare decat numarul total de
	// postari, afisam toate postarile curente
	if (feed_size > posts->nr_posts)
		feed_size = posts->nr_posts;

	// parcurgem postarile in ordine inversa
	// (de la cea mai recenta la cea mai veche)
	for (int i = posts->nr_posts - 1; i >= 0 && feed_size; i--) {
		uint16_t user_id = ((info_t *)posts->posts[i]->root->data)->user_id;
		aux = graph->neighbors[id]->head;
		int ok = 0;
		// parcurgem lista de prieteni a utilizatorului
		while (aux && !ok) {
			// daca cel care a facut postarea este prieten cu utilizatorul dat
			// sau este chiar utilizatorul dat, afisam postarea
			if (*((uint16_t *)aux->data) == user_id || user_id == id) {
				printf("%s: \"%s\"\n", get_user_name(user_id),
					   ((info_t *)posts->posts[i]->root->data)->title);
				// decrementam numarul de postari ramase de afisat
				feed_size--;
				ok = 1;
			}
			aux = aux->next;
		}
	}
}

void
view_posts(post_array_t *posts, char *name)
{
	// obtinem id-ul utilizatorului
	uint16_t id = get_user_id(name);

	// parcurgem toate postarile si le afisam pe cele ale utilizatorului
	for (unsigned int i = 0; i < posts->nr_posts; i++) {
		uint16_t user_id = ((info_t *)posts->posts[i]->root->data)->user_id;
		if (user_id == id) {
			printf("Posted: \"%s\"\n",
				   ((info_t *)posts->posts[i]->root->data)->title);
		}
	}

	// parcurgem din nou toate postarile pentru a afisa reposturile
	// facute de utilizator
	for (unsigned int i = 0; i < posts->nr_posts; i++)
		view_reposts(posts->posts[i]->root, id);
}

void
view_reposts(post_t *post, uint16_t id)
{
	// daca postarea este nula, iesim din functie
	if (!post)
		return;

	// daca id-ul utilizatorului care a repostat este id-ul utilizatorului
	// curent si postarea este o repostare, afisam postarea originala
	if (((info_t *)post->data)->user_id == id && post->parent_id != 0)
		printf("Reposted: \"%s\"\n", ((info_t *)post->data)->title);

	// parcurgem toate repostarile postarii curente
	for (unsigned int i = 0; i < post->nr_children; i++)
		if (post->children[i])
			view_reposts(post->children[i], id);
}

void
friends_repost(list_graph_t *graph, post_t *post, uint16_t id)
{
	// daca postarea este nula, iesim din functie
	if (!post)
		return;

	// daca postarea este o repostare
	if (post->parent_id != 0) {
		// obtinem id-ul utilizatorului care a repostat
		uint16_t user_id = ((info_t *)post->data)->user_id;
		dll_node_t *aux = graph->neighbors[id]->head;
		int ok = 0;
		// parcurgem lista de prieteni a utilizatorului
		while (aux && ok == 0) {
			// daca cel care a repostat este prieten cu utilizatorul dat
			// il afisam
			if (*((uint16_t *)aux->data) == user_id) {
				printf("%s\n", get_user_name(user_id));
				ok = 1;
			}
			aux = aux->next;
		}
	}

    // parcurgem toate repostarile postarii curente
	for (unsigned int i = 0; i < post->nr_children; i++)
		if (post->children[i])
			friends_repost(graph, post->children[i], id);
}

void
clique(list_graph_t *graph, uint16_t id)
{
	// creem o lista noua pentru a stoca clica
	doubly_linked_list_t *clique = dll_create(sizeof(uint16_t));
	dll_node_t *aux;
	// parcurgem fiecare utilizator
	for (int i = 0; i < graph->nodes; i++) {
		if ((lg_has_edge(graph, i, id) && lg_has_edge(graph, id, i))) {
			// daca utilizatorul curent si cel dat sunt prieteni, cream o lista
			// noua pentru a stoca grupul de prieteni al utilizatorului curent
			doubly_linked_list_t *group = dll_create(sizeof(uint16_t));

			// adaugam utilizatorul curent in aceasta lista
			dll_add_nth_node(group, group->size, &i, sizeof(uint16_t));

			// adaugam in ordinea crescatoare a id-urilor toti prietenii
			// utilizatorului curent in lista
			aux = graph->neighbors[i]->head;
			for (unsigned int j = 0; j < graph->neighbors[i]->size; j++) {
				unsigned int pos = add_ascending(group, *(uint16_t *)aux->data);
				dll_add_nth_node(group, pos, aux->data, sizeof(uint16_t));
				aux = aux->next;
			}

			// verificam daca grupul curent este o clica
			int ok = 1;
			// resetam pointerul la inceputul listei de vecini
			aux = group->head;
			// parcurgem fiecare pereche din lista pentru a verifica daca
			// oricare doi utilizatori sunt prieteni intre ei
			for (unsigned int j = 0; j < group->size && ok; j++) {
				dll_node_t *aux2 = aux->next;
				for (unsigned int k = j + 1; k < group->size; k++) {
					// daca nu exista o muchie intre elem. curent si urmatorul
					// gruparea curenta nu este o clica
					if (!lg_has_edge(graph, *(uint16_t *)aux->data,
									 *(uint16_t *)aux2->data) ||
						!lg_has_edge(graph, *(uint16_t *)aux2->data,
									 *(uint16_t *)aux->data)) {
						ok = 0;
						break;
					}
					aux2 = aux2->next;
				}
				aux = aux->next;
			}
			// daca gruparea curenta este o clica si dimensiunea ei este mai
			// mare decat cea mai mare clica gasita pana acum, o actualizam
			if (ok && group->size > clique->size) {
				dll_free(&clique);
				clique = group;
			} else {
				dll_free(&group);
			}
		}
	}

	// resetam pointerul la inceputul listei de vecini
	aux = clique->head;
	// parcurgem clica pentru a afisa toti membrii acesteia
	printf("The closest friend group of %s is:\n", get_user_name(id));
	for (unsigned int i = 0; i < clique->size; i++) {
		printf("%s\n", get_user_name(*(uint16_t *)aux->data));
		aux = aux->next;
	}
	dll_free(&clique);
}

void
handle_input_feed(char *input, list_graph_t *graph, post_array_t *posts)
{
	char *commands = strdup(input);
	char *original_commands = commands;
	char *cmd = strtok(commands, "\n ");
	commands = strtok(NULL, "\n");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		// prelucrare de input pentru comanda feed
		char *name = strtok(commands, "\n ");
		unsigned int feed_size = atoi(strtok(NULL, "\n "));

		feed(graph, posts, name, feed_size);
	} else if (!strcmp(cmd, "view-profile")) {
		// prelucrare de input pentru comanda view-profile
		char *name = strtok(commands, "\n ");
		view_posts(posts, name);
	} else if (!strcmp(cmd, "friends-repost")) {
		// prelucrare de input pentru comanda friends-repost
		char *name = strtok(commands, "\n ");
		unsigned int post_id = atoi(strtok(NULL, "\n "));

		uint16_t index = get_index(post_id, posts);
		friends_repost(graph, posts->posts[index]->root, get_user_id(name));
	} else if (!strcmp(cmd, "common-group")) {
		// prelucrare de input pentru comanda common-group
		char *name = strtok(commands, "\n ");
		clique(graph, get_user_id(name));
	}

	commands = strtok(NULL, "\n");
	if (original_commands)
		free(original_commands);
}
