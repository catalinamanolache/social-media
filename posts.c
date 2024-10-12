#include "posts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "users.h"

post_array_t *
init_post_array(void)
{
	post_array_t *array = malloc(sizeof(post_array_t));
	DIE(!array, "malloc failed");

	array->posts = NULL;
	array->nr_posts = 0;
	array->current_id = 0;
	return array;
}

post_tree_t *
init_post_tree(unsigned int data_size)
{
	post_tree_t *tree = malloc(sizeof(post_tree_t));
	DIE(!tree, "malloc failed");

	tree->root = NULL;
	tree->data_size = data_size;
	return tree;
}

post_t *
init_post(unsigned int data_size, void *data, uint16_t parent_id)
{
	post_t *post = malloc(sizeof(post_t));
	DIE(!post, "malloc failed");

	post->children = NULL;
	post->nr_children = 0;
	post->parent_id = parent_id;

	post->data = malloc(data_size);
	DIE(!post->data, "malloc failed");
	memcpy(post->data, data, data_size);
	return post;
}

void
post_array_free(post_array_t **array)
{
	if (*array) {
		if ((*array)->posts) {
			for (unsigned int i = 0; i < (*array)->nr_posts; i++) {
				free_likes(&(*array)->posts[i]->root);
				post_tree_free(&(*array)->posts[i]);
			}
			free((*array)->posts);
		}
		free(*array);
		*array = NULL;
	}
}

void
post_tree_free(post_tree_t **tree)
{
	if (*tree) {
		if ((*tree)->root)
			post_free(&(*tree)->root);
		free(*tree);
		*tree = NULL;
	}
}

void
free_likes(post_t **post)
{
	if (*post) {
		if ((*post)->children) {
			for (unsigned int i = 0; i < (*post)->nr_children; i++)
				free_likes(&(*post)->children[i]);
		}
		if (((info_t *)(*post)->data)->likes)
			free(((info_t *)(*post)->data)->likes);
	}
}

void
post_free(post_t **post)
{
	if (*post) {
		if ((*post)->children) {
			for (unsigned int i = 0; i < (*post)->nr_children; i++)
				post_free(&(*post)->children[i]);
			free((*post)->children);
		}
		if ((*post)->data)
			free((*post)->data);
		free(*post);
		*post = NULL;
	}
}

info_t *
create_post(char *name, char *post, post_array_t **post_array)
{
	uint16_t id = get_user_id(name);
	info_t *info = malloc(sizeof(info_t));
	DIE(!info, "malloc failed");

	(*post_array)->current_id++;
	info->id = (*post_array)->current_id;

	strcpy(info->title, post);
	info->user_id = id;
	info->likes = NULL;
	info->nr_likes = 0;
	return info;
}

void
add_post(char *name, char *post, post_array_t **post_array)
{
	// adaugam o postare noua in vectorul de postari
	info_t *info = create_post(name, post, post_array);

	post_t *new_post = init_post(sizeof(info_t), info, 0);
	free(info);

	post_tree_t *new_tree = init_post_tree(sizeof(uint16_t));
	new_tree->root = new_post;

	// realocam vectorul de postari
	(*post_array)->nr_posts++;
	post_tree_t **aux =
		realloc((*post_array)->posts,
				((*post_array)->nr_posts) * sizeof(post_tree_t *));
	DIE(!aux, "realloc failed");
	(*post_array)->posts = aux;

	(*post_array)->posts[(*post_array)->nr_posts - 1] = new_tree;
	printf("Created \"%s\" for %s\n", post, name);
}

void
add_repost(char *name, uint16_t post_id, uint16_t repost_id,
		   post_array_t **post_array, post_t **post)
{
	// daca postarea nu exista, iesim din functie
	if (!(*post))
		return;

	if (repost_id != 0) {
		// daca este o repostare la o repostare
		if (repost_id == ((info_t *)(*post)->data)->id) {
			// daca am gasit postarea la care vrem sa adaugam repostarea
			// o cream si o adaugam in vectorul de copii
			info_t *info =
				create_post(name, ((info_t *)(*post)->data)->title, post_array);
			post_t *new_post = init_post(sizeof(info_t), info, repost_id);
			free(info);

			(*post)->nr_children++;
			post_t **aux = realloc((*post)->children,
								   (*post)->nr_children * sizeof(post_t *));
			DIE(!aux, "realloc failed");
			(*post)->children = aux;

			(*post)->children[(*post)->nr_children - 1] = new_post;
			printf("Created repost #%u for %s\n", (*post_array)->current_id,
				   name);
			return;
		}
	} else {
		// daca este o repostare la o postare
		// o cream si o adaugam in vectorul de copii
		info_t *info =
			create_post(name, ((info_t *)(*post)->data)->title, post_array);
		post_t *new_post = init_post(sizeof(info_t), info, post_id);
		free(info);

		(*post)->nr_children++;
		post_t **aux =
			realloc((*post)->children, (*post)->nr_children * sizeof(post_t *));
		DIE(!aux, "realloc failed");
		(*post)->children = aux;

		(*post)->children[(*post)->nr_children - 1] = new_post;
		printf("Created repost #%u for %s\n", (*post_array)->current_id, name);
		return;
	}

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < (*post)->nr_children; i++)
		if ((*post)->children[i])
			add_repost(name, post_id, repost_id, post_array,
					   &(*post)->children[i]);
}

void
print_reposts_post(post_t *post, uint16_t post_id)
{
	// daca postarea nu exista, iesim din functie
	if (!post)
		return;

	// realizam verificarea pentru a nu se afisa pe sine
	if (post_id != ((info_t *)post->data)->id)
		printf("Repost #%u by %s\n", ((info_t *)post->data)->id,
			   get_user_name(((info_t *)post->data)->user_id));

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < post->nr_children; i++)
		print_reposts_post(post->children[i], post_id);
}

void
print_reposts_repost(post_t *post)
{
	// daca postarea nu exista, iesim din functie
	if (!post)
		return;

	// afisam repostarea
	printf("Repost #%u by %s\n", ((info_t *)post->data)->id,
		   get_user_name(((info_t *)post->data)->user_id));

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < post->nr_children; i++)
		print_reposts_repost(post->children[i]);
}

void
get_reposts(uint16_t post_id, uint16_t repost_id,
			post_array_t **post_array, post_t **post)
{
	// daca postarea nu exista, iesim din functie
	if (!(*post))
		return;

	// daca am gasit postarea la care vrem sa afisam repostarile
	if (repost_id == ((info_t *)(*post)->data)->id || repost_id == 0) {
		if (repost_id == 0) {
			// afisam repostarile postarii
			printf("\"%s\" - Post by %s\n", ((info_t *)(*post)->data)->title,
				   get_user_name(((info_t *)(*post)->data)->user_id));
			print_reposts_post(*post, post_id);
		} else {
			// afisam repostarile repostarii
			print_reposts_repost(*post);
		}
		return;
	}

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < (*post)->nr_children; i++)
		if ((*post)->children[i])
			get_reposts(post_id, repost_id, post_array, &(*post)->children[i]);
}

post_t *
common_repost(post_t *root, uint16_t id1, uint16_t id2)
{
	// daca postarea nu exista, iesim din functie
	if (!root)
		return NULL;

	if (((info_t *)root->data)->id == id1 || ((info_t *)root->data)->id == id2)
		return root;

	// initializam postarea comuna
	post_t *common = NULL;

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < root->nr_children; i++) {
		post_t *aux = common_repost(root->children[i], id1, id2);
		if (aux) {
			// daca deja a fost gasita o postare comuna, se returneaza
			// postarea curenta
			if (common)
				return root;

			// altfel se retine postarea gasita
			common = aux;
		}
	}
	return common;
}

void
print_common_repost(uint16_t index, uint16_t repost_id1, uint16_t repost_id2,
					post_array_t *posts)
{
	post_t *common =
			common_repost(posts->posts[index]->root, repost_id1, repost_id2);
	if (common)
		printf("The first common repost of %u and %u is %u\n", repost_id1,
			   repost_id2, ((info_t *)common->data)->id);
}

void
like_repost(char *name, uint16_t repost_id, post_t **post)
{
	// daca postarea nu exista, iesim din functie
	if (!(*post))
		return;

	// daca am gasit repostarea la care vrem sa adaugam like-ul
	if (((info_t *)(*post)->data)->id == repost_id) {
		int check_like = like_post(name, post);
		// verificam daca se da like sau dislike
		if (!check_like)
			printf("User %s liked repost \"%s\"\n", name,
				   ((info_t *)(*post)->data)->title);
		else
			printf("User %s unliked repost \"%s\"\n", name,
				   ((info_t *)(*post)->data)->title);
		return;
	}

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < (*post)->nr_children; i++)
		if ((*post)->children[i])
			like_repost(name, repost_id, &(*post)->children[i]);
}

int
like_post(char *name, post_t **post)
{
	unsigned int i = 0, nr_likes = ((info_t *)(*post)->data)->nr_likes;
	// se cauta utilizatorul in vectorul de like-uri
	for (i = 0; i < nr_likes; i++)
		if (((info_t *)(*post)->data)->likes[i] == get_user_id(name))
			break;

	if (i == nr_likes) {
		// acesta nu a fost gasit, deci nu a dat like la postare
		// il adaugam in vectorul de like-uri
		((info_t *)(*post)->data)->nr_likes++;
		nr_likes++;

		uint16_t *aux = realloc(((info_t *)(*post)->data)->likes,
								(nr_likes) * sizeof(uint16_t));
		DIE(!aux, "realloc failed");
		((info_t *)(*post)->data)->likes = aux;
		((info_t *)(*post)->data)->likes[nr_likes - 1] = get_user_id(name);
		return 0;
	}

	// s-a gasit in vectorul de like-uri, deci il eliminam (se da dislike)
	for (unsigned int j = i; j < nr_likes - 1; j++)
		((info_t *)(*post)->data)->likes[j] =
			((info_t *)(*post)->data)->likes[j + 1];

	((info_t *)(*post)->data)->nr_likes--;
	nr_likes--;

	if (nr_likes) {
		uint16_t *aux = realloc(((info_t *)(*post)->data)->likes,
								(nr_likes) * sizeof(uint16_t));
		DIE(!aux, "realloc failed");
		((info_t *)(*post)->data)->likes = aux;
	}
	return 1;
}

void
print_likes(uint16_t repost_id, char *name, uint16_t index,
			post_array_t *posts)
{
	int check_like;
	if (repost_id) {
		like_repost(name, repost_id, &posts->posts[index]->root);
	} else {
		check_like = like_post(name, &posts->posts[index]->root);
		if (!check_like)
			printf("User %s liked post \"%s\"\n", name,
				   ((info_t *)(posts->posts[index]->root)->data)->title);
		else
			printf("User %s unliked post \"%s\"\n", name,
				   ((info_t *)(posts->posts[index]->root)->data)->title);
	}
}

void
get_post_likes(post_t *post)
{
	// daca postarea nu exista, iesim din functie
	if (!post)
		return;

	// afisam numarul de like-uri ale postarii
	printf("Post \"%s\" has %u likes\n", ((info_t *)post->data)->title,
		   ((info_t *)post->data)->nr_likes);
}

void
get_repost_likes(uint16_t repost_id, post_t *post)
{
	// daca postarea nu exista, iesim din functie
	if (!post)
		return;

	// am gasit repostarea la care vrem sa afisam like-urile
	if (((info_t *)post->data)->id == repost_id) {
		printf("Repost #%u has %u likes\n", repost_id,
			   ((info_t *)post->data)->nr_likes);
		return;
	}

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < post->nr_children; i++)
		get_repost_likes(repost_id, post->children[i]);
}

void
print_get_likes(uint16_t repost_id, unsigned int index, post_array_t *posts)
{
	if (repost_id)
		get_repost_likes(repost_id, posts->posts[index]->root);
	else
		get_post_likes(posts->posts[index]->root);
}

void
max_likes_repost(unsigned int *max_likes, uint16_t *id, post_t *post)
{
	// daca postarea nu exista, iesim din functie
	if (!post)
		return;

	// daca postarea curenta este o repostare si are mai multe like-uri decat
	// repostarea cu cele mai multe like-uri de pana acum, o retinem
	if (((info_t *)post->data)->nr_likes > *max_likes && post->parent_id != 0) {
		*max_likes = ((info_t *)post->data)->nr_likes;
		*id = ((info_t *)post->data)->id;
	} else if (((info_t *)post->data)->nr_likes == *max_likes &&
			   post->parent_id != 0) {
		// in caz de egalitate, retinem repostarea cu id-ul mai mic
		if (((info_t *)post->data)->id < *id)
			*id = ((info_t *)post->data)->id;
	}

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < post->nr_children; i++)
		max_likes_repost(max_likes, id, post->children[i]);
}

void
ratio(post_t *post)
{
	// gasim numarul de like-uri al postarii si al repostarilor
	unsigned int post_likes = ((info_t *)post->data)->nr_likes;
	unsigned int max_likes = 0;
	uint16_t id = 0;
	// gasim repostarea cu cele mai multe like-uri
	max_likes_repost(&max_likes, &id, post);
	if (max_likes <= post_likes) {
		// postarea originala are mai multe like-uri decat repostarea
		printf("The original post is the highest rated\n");
	} else {
		// repostarea are mai multe like-uri decat postarea originala,
		// deci se da ratio
		printf("Post %u got ratio'd by repost %u\n",
			   ((info_t *)post->data)->id, id);
	}
}

void
delete_repost(uint16_t id, post_t **post)
{
	// daca postarea nu exista, iesim din functie
	if (!(*post))
		return;

	// am gasit postarea pe care vrem sa o stergem
	if (((info_t *)(*post)->data)->id == id) {
		delete_post(post);
		return;
	}

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < (*post)->nr_children; i++)
		delete_repost(id, &(*post)->children[i]);
}

void
delete_post(post_t **post)
{
	// daca postarea nu exista, iesim din functie
	if (!(*post))
		return;

	// parcurgem toti copiii postarii curente
	for (unsigned int i = 0; i < (*post)->nr_children; i++)
		delete_post(&(*post)->children[i]);

	// eliberam memoria alocata pentru postare
	free_likes(post);
	post_free(post);
}

void
print_delete(uint16_t index, uint16_t repost_id, post_array_t *posts)
{
	char *old_post =
		strdup(((info_t *)(posts->posts[index]->root)->data)->title);

	if (repost_id != 0) {
		delete_repost(repost_id, &posts->posts[index]->root);
		printf("Deleted repost #%u of post \"%s\"\n", repost_id, old_post);
	} else {
		delete_post(&posts->posts[index]->root);
		posts->nr_posts--;
		printf("Deleted \"%s\"\n", old_post);
	}
	free(old_post);
}

uint16_t
get_index(uint16_t id, post_array_t *posts)
{
	for (unsigned int i = 0; i < posts->nr_posts; i++)
		if (((info_t *)posts->posts[i]->root->data)->id == id)
			return i;
	return 0;
}

void
handle_input_posts(char *input, post_array_t *posts)
{
	char *commands = strdup(input);
	char *original_commands = commands;
	char *cmd = strtok(commands, "\n ");
	commands = strtok(NULL, "\n");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *name = strtok(commands, "\n ");
		char *post = strtok(NULL, "\"");
		add_post(name, post, &posts);
	} else if (!strcmp(cmd, "repost")) {
		char *name = strtok(commands, "\n ");

		uint16_t post_id = atoi(strtok(NULL, "\n "));
		uint16_t index = get_index(post_id, posts);
		commands = strtok(NULL, "\n");

		uint16_t repost_id = 0;
		if (commands)
			repost_id = atoi(commands);

		add_repost(name, post_id, repost_id, &posts,
				   &posts->posts[index]->root);
	} else if (!strcmp(cmd, "common-repost")) {
		uint16_t post_id = atoi(strtok(commands, "\n "));
		uint16_t repost_id1 = atoi(strtok(NULL, "\n "));
		uint16_t repost_id2 = atoi(strtok(NULL, "\n "));

		uint16_t index = get_index(post_id, posts);
		print_common_repost(index, repost_id1, repost_id2, posts);
	} else if (!strcmp(cmd, "like")) {
		char *name = strtok(commands, "\n ");

		uint16_t post_id = atoi(strtok(NULL, "\n "));
		uint16_t index = get_index(post_id, posts);
		commands = strtok(NULL, "\n");

		uint16_t repost_id = 0;
		if (commands)
			repost_id = atoi(commands);
		print_likes(repost_id, name, index, posts);
	} else if (!strcmp(cmd, "ratio")) {
		uint16_t post_id = atoi(strtok(commands, "\n "));
		uint16_t index = get_index(post_id, posts);
		ratio(posts->posts[index]->root);
	} else if (!strcmp(cmd, "delete")) {
		uint16_t post_id = atoi(strtok(commands, "\n "));
		commands = strtok(NULL, "\n");

		uint16_t repost_id = 0;
		if (commands)
			repost_id = atoi(commands);

		uint16_t index = get_index(post_id, posts);
		print_delete(index, repost_id, posts);
	} else if (!strcmp(cmd, "get-likes")) {
		uint16_t post_id = atoi(strtok(commands, "\n "));
		uint16_t index = get_index(post_id, posts);
		commands = strtok(NULL, "\n");

		uint16_t repost_id = 0;
		if (commands)
			repost_id = atoi(commands);
		print_get_likes(repost_id, index, posts);
	} else if (!strcmp(cmd, "get-reposts")) {
		uint16_t post_id = atoi(strtok(commands, "\n "));
		uint16_t index = get_index(post_id, posts);
		commands = strtok(NULL, "\n");

		uint16_t repost_id = 0;
		if (commands)
			repost_id = atoi(commands);
		get_reposts(post_id, repost_id, &posts, &posts->posts[index]->root);
	}
	commands = strtok(NULL, "\n");
	if (original_commands)
		free(original_commands);
}
