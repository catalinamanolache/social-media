#ifndef FEED_H
#define FEED_H

#include "graph.h"
#include "posts.h"
#include "friends.h"

/**
 * Functia afiseaza cele mai recente feed_size postari ale unui utilizator
 * si ale prietenilor sai.
*/
void
feed(list_graph_t *graph, post_array_t *posts, char *name,
	 unsigned int feed_size);

/**
 * Functia afiseaza toate postarile unui utilizator si reposturile acestuia.
*/
void
view_posts(post_array_t *posts, char *name);

/*
 * Functia afiseaza toate reposturile unui utilizator.
*/
void
view_reposts(post_t *post, uint16_t id);

/**
 * Functia afiseaza toti prietenii unui utilizator care au dat repost la
 * postarea cu id-ul primit.
*/
void
friends_repost(list_graph_t *graph, post_t *post, uint16_t id);

/**
 * Functia afiseaza cel mai mare grup de prieteni care il contine pe
 * utilizatorul cu id-ul primit.
*/
void
clique(list_graph_t *graph, uint16_t id);

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void
handle_input_feed(char *input, list_graph_t *graph,  post_array_t *posts);
#endif // FEED_H
