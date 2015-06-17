/*
 * dblink_list.h
 *
 *  Created on: Jun 3, 2015
 *      Author: hoeselm
 */

#ifndef DBLINK_LIST_H_
#define DBLINK_LIST_H_

struct Node {
	long data;
	struct Node* next;
	struct Node* previous;
};
struct List {
	struct Node* head;
	struct Node* tail;
};

struct List *new_empty_list();

void clear(struct List *list);

void insert_value(struct List *list, long value);

void delete_value(struct List *list, long value);

unsigned int search (struct List *list, long value);

unsigned int list_size(struct List *list);

struct List *create_sorted_list(long* value_array, unsigned int array_length);

void print_list(struct List *list);

void print_reverse(struct List *list);

#endif /* DBLINK_LIST_H_ */
