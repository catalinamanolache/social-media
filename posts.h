#ifndef POSTS_H
#define POSTS_H

#include "graph.h"

typedef struct info_t {
	// structura pentru informatiile unei postari
	uint16_t id;
	char title[281];
	uint16_t user_id;
	uint16_t *likes;
	unsigned int nr_likes;
} info_t;

typedef struct post_t post_t;
typedef struct post_t {
	// structura pentru o postare
	post_t **children;
	unsigned int nr_children;
	// de la ce postare vine repostarea
	uint16_t parent_id;
	// informatiile postarii
	void *data;
} post_t;

typedef struct post_tree_t {
	// structura pentru arborele de postari
	post_t *root;
	unsigned int data_size;
} post_tree_t;

typedef struct post_array_t {
	// structura pentru vectorul de postari
	post_tree_t **posts;
	unsigned int nr_posts;
	uint16_t current_id;
} post_array_t;

/**
 * Functia initializeaza un vector de postari.
 */
post_array_t *
init_post_array(void);

/**
 * Functia initializeaza un arbore de postari.
 */
post_tree_t *
init_post_tree(unsigned int data_size);

/**
 * Functia initializeaza o postare.
 */
post_t *
init_post(unsigned int data_size, void *data, uint16_t parent_id);

/**
 * Functia elibereaza memoria alocata pentru un vector de postari.
 */
void
post_array_free(post_array_t **array);

/**
 * Functia elibereaza memoria alocata pentru un arbore al unei postari.
 */
void
post_tree_free(post_tree_t **tree);

/**
 * Functia elibereaza memoria alocata pentru vectorul de like-uri ale
 * unei postari.
 */
void
free_likes(post_t **post);

/**
 * Functia elibereaza memoria alocata pentru un nod din arborele unei postari.
 */
void
post_free(post_t **post);

/**
 * Functia initializeaza informatiile unei postari noi.
 */
info_t *
create_post(char *name, char *post, post_array_t **post_array);

/**
 * Functia adauga o postare in vectorul de postari.
 */
void
add_post(char *name, char *post, post_array_t **post_array);

/**
 * Functia adauga un repost la o postare.
 */
void
add_repost(char *name, uint16_t post_id, uint16_t repost_id,
		   post_array_t **post_array, post_t **post);

/**
 * Functia afiseaza toate repostarile postarii unui utilizator.
 */
void
print_reposts_post(post_t *post, uint16_t post_id);

/**
 * Functia afiseaza toate repostarile repostarii unui utilizator.
 */
void
print_reposts_repost(post_t *post);

/**
 * Functia afiseaza toate repostarile postarii/repostarii unui utilizator.
 */
void
get_reposts(uint16_t post_id, uint16_t repost_id,
			post_array_t **post_array, post_t **post);

/**
 * Functia gaseste primul repost comun a doua repostari.
 */
post_t *
common_repost(post_t *root, uint16_t id1, uint16_t id2);

/**
 * Functia printeaza mesajul pentru primul repost comun a doua repostari.
 */
void
print_common_repost(uint16_t index, uint16_t repost_id1, uint16_t repost_id2,
					post_array_t *posts);

/**
 * Functia adauga un like la o repostare.
 */
void
like_repost(char *name, uint16_t repost_id, post_t **post);

/**
 * Functia adauga un like la o postare.
 */
int
like_post(char *name, post_t **post);

/**
 * Functia printeaza mesajul pentru like-urile unei postari.
 */
void
print_likes(uint16_t repost_id, char *name, uint16_t index,
			post_array_t *posts);

/**
 * Functia afiseaza numarul de like-uri ale unei postari.
 */
void
get_post_likes(post_t *post);

/**
 * Functia afiseaza numarul de like-uri ale unei repostari.
 */
void
get_repost_likes(uint16_t repost_id, post_t *post);

/**
 * Functia printeaza mesajul pentru get-likes.
 */
void
print_get_likes(uint16_t repost_id, unsigned int index, post_array_t *posts);

/**
 * Functia gaseste repostarea cu cele mai multe like-uri.
 */
void
max_likes_repost(unsigned int *max_likes, uint16_t *id, post_t *post);

/**
 * Functia verifica daca o postare si-a luat ratio.
 */
void
ratio(post_t *post);

/**
 * Functia sterge o repostare, precum si toti copiii ei.
 */
void
delete_repost(uint16_t id, post_t **post);

/**
 * Functia sterge o postare, precum si toti copiii ei.
 */
void
delete_post(post_t **post);

/**
 * Functia printeaza mesajul pentru delete.
 */
void
print_delete(uint16_t index, uint16_t repost_id, post_array_t *posts);

/**
 * Functia gaseste indexul postarii cu id-ul dat ca parametru
 * din vectorul de postari.
 */
uint16_t
get_index(uint16_t id, post_array_t *posts);

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, post_array_t *posts);

#endif // POSTS_H
