/*
 * Copyright (c) 2024, <Diaconescu Stefania Clara 313CA>
 */

#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_t {
	// Dimensiunea maxima a cozii.
	unsigned int max_size;
	// Dimensiunea cozii.
	unsigned int size;
	// Dimensiunea in bytes a tipului de date stocat in coada.
	unsigned int data_size;
	// Indexul de la care se vor efectua operatiile de front si dequeue.
	unsigned int read_idx;
	// Indexul de la care se vor efectua operatiile de enqueue.
	unsigned int write_idx;
	// Bufferul ce stocheaza elementele cozii.
	void **buff;
} queue_t;

/**
 * Functia aloca si initializeaza o coada.
 */
queue_t *
q_create(unsigned int data_size, unsigned int max_size);

/**
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
unsigned int
q_get_size(queue_t *q);

/**
 *  Functia intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q);

/**
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *
q_front(queue_t *q);

/**
 * Functia elimina ultimul element din coada.
 */
int
q_dequeue(queue_t *q);

/**
 * Functia adauga un nou element in coada.
 */
int
q_enqueue(queue_t *q, void *new_data);

/**
 * Functia elimina toate elementele din coada primita ca parametru.
 */
void
q_clear(queue_t *q);

/**
 * Functia elibereaza memoria ocupata de coada.
 */
void
q_free(queue_t *q);
#endif
