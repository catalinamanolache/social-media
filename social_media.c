/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "graph.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

list_graph_t *
create_network(void)
{
	list_graph_t *graph = lg_create(MAX_PEOPLE, sizeof(uint16_t));
	return graph;
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();
	list_graph_t *graph = create_network();
	post_array_t *posts = init_post_array();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graph);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, posts);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, graph, posts);
		#endif
	}
	lg_free(&graph);
	post_array_free(&posts);

	free_users();
	free(input);

	return 0;
}
