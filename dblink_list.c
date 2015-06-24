#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dblink_list.h"

struct List *new_empty_list() {
	struct List *new_List; //Creates a new List
	new_List = (struct List*) malloc(sizeof(struct List)); //Allocates the memory
	new_List->head=0;
	new_List->tail=0;

	return new_List;
}

void clear(struct List *list) {
	/*
	 * This function frees any objects within a given list and sets the head and tail pointers back to zero.
	 */

	//Creates 2 Node Pointers that will iterate through the list
	struct Node* iterator_2;
	struct Node* iterator_2_next;

	iterator_2 = list->head; //Assigns the current pointer to the head of the list

	while (1) {
		iterator_2_next = iterator_2->next;
		free((void*) iterator_2); //Frees the current node
		if (iterator_2_next == list->tail) { //If the next node is the tail
			free((void*) list->tail);
			printf("List cleared!\n");
			list->head=0;
			list->tail=0;
			return;
		}
		iterator_2 = iterator_2_next; //Iterates forward through the list
	}
}

void insert_value(struct List *list, long value) {
	/*GENERAL STRUCTURE
	 *
	 * The function checks whether there is no head as of yet, if so it inserts a new node and value as head.
	 * If there is no tail yet, the function checks whether the head is smaller than the value that is to be inserted, if so, the new value becomes the tail.
	 * If not, the old head becomes the tail and the new value the head
	 *
	 * HEAD		- 		TAIL
	 * 0				0
	 *
	 * VALUE
	 * 2
	 * -> Insert Value into head
	 *
	 * ---------------------------------------------------------------
	 *
	 * THEN	(1)							OR (2)
	 * HEAD		- 		TAIL			HEAD		- 		TAIL
	 * 2				0				2					0
	 *
	 * VALUE							VALUE
	 * 3								1
	 *-> Insert value into tail			-> Swap head to tail and insert new value
	 *
	 * HEAD		- 		TAIL			NEW HEAD	OLD HEAD	TAIL
	 * 2				3		 		1			2			0
	 *												 \
	 *									->			  \
	 *												   \
	 *									HEAD		-  	\	TAIL
	 *									1				 -->2
	 *---------------------------------------------------------------
	 *
	 * When a new value gets inserted, it then checks whether it's smaller than the head or larger than the tail. If so, (2) is used for head/tail, otherwise the value
	 * will get inserted normally in between.
	 */

	struct Node* new_Node;
	new_Node = (struct Node*) malloc(sizeof(struct Node));
	new_Node->data = value;

	/*
	 *EDGE CASES LOOKING FOR HEAD AND TAIL START HERE
	 */

	//First check: Is there a head
	if(list->head == 0){
		list->head = new_Node;
		new_Node->next = list->tail;
		new_Node->previous = 0;
		return;
	}
	//Second check: Is there a tail
	if(list->tail == 0){
		//Sub-check: Is value smaller or larger than the head value
		if(list->head->data < value){
			//The new value will get inserted as tail
			list->tail = new_Node;
			new_Node->previous = list->head;
			new_Node->next = 0;
			list->head->next = new_Node;
			return;
		}
		else{
			//The new value will become the head and the old, larger one the tail
			//Relocation of the old head
			list->tail = list->head;
			list->tail->next = 0;
			list->tail->previous = new_Node;

			//Assigning the new head
			list->head = new_Node;
			list->head->next = list->tail;
			list->head->previous =0;
			return;
		}
	}

	/*
	 * END OF HEAD-TAIL-LOOKUP EDGE CASES
	 */

	/*
	 * EDGE CASES LOOKING FOR VALUES OUTSIDE OF HEAD->TAIL RANGE START HERE
	 */

	//Both head and tail exist
	//Check whether the value is within the head->tail range
	if(value < list->head->data){
		//The value is smaller than the current head

		struct Node* tmp_ptr;
		tmp_ptr = list->head;

		list->head->previous = new_Node;
		new_Node->previous = 0;
		new_Node->next = tmp_ptr;
		list->head = new_Node;
		return;
	}
	if(value > list->tail->data){
		//The value is larger than the current tail

		struct Node* tmp_ptr;
		tmp_ptr = list->tail;

		list->tail->next = new_Node;
		new_Node->next = 0;
		new_Node->previous = tmp_ptr;
		list->tail = new_Node;
		return;
	}

	/*
	 * END OF EDGE CASES SECTION
	 */

	/*
	 * This procedure iterates from small to large. Every time, it is checked whether the value is smaller or equal to the following one. Since the value
	 * is automatically larger than the head, we can start at head.next and then go through until we hit tail
	 */

	struct Node* iterator;
	iterator = list->head->next;

	while(1){
		if(value <= iterator->data){
			new_Node->next = iterator;
			new_Node->previous = iterator->previous;

			iterator->previous->next = new_Node;
			iterator->previous = new_Node;
			return;
		}
		iterator = iterator->next;
	}
}

void delete_value(struct List *list, long value) {
	/*
	 * This function looks for a specific value within a function. If the value is found, it checks for edge cases and deletes the value, if not, it will return with an error.
	 */
	struct Node* iterator_3;
	iterator_3 = list->head;

	while (1) {
		if (iterator_3->data == value) { //Called if the value is found
			/*
			 * EDGE CASES START HERE
			 */

			if(iterator_3 == list->head){
				//If the deleting value is the head of the list:
				list->head = iterator_3->next;
				iterator_3->previous = 0;
				free((void*) iterator_3);
				printf("Value deleted\n");
				return;
			}
			if(iterator_3 == list->tail){
				//If the deleting value is the tail of the list:
				list->tail = iterator_3->previous;
				iterator_3->next=0;
				free((void*) iterator_3);
				printf("Value deleted\n");
				return;
			}

			/*
			 * END OF EDGE CASES
			 */

			//Redirects the pointers
			iterator_3->previous->next = iterator_3->next;
			iterator_3->next->previous = iterator_3->previous;

			//Frees the node in question
			free((void*) iterator_3);
			printf("Value deleted\n");
			return;
		}
		//Failsafe for the end of the List
		if (iterator_3 == list->tail) {
			printf("Value not found!\n");
			return;
		}
		iterator_3 = iterator_3->next;
	}
}

unsigned int search(struct List *list, long value) {
	/*
	 * This function will search through the list until it finds a given value. It then prints the node number where the value was found
	 */

	struct Node* iterator_4;
	iterator_4 = list->head;
	unsigned int counter = 1;

	while (1) {
		if (iterator_4->data == value) { //Called if the value is found
			printf("Value found at node: %u\n", counter);
			return counter;
		}
		//Failsafe for the end of the List
		if (iterator_4 == list->tail) {
			printf("Value not found!\n");
			return 0;
		}
		iterator_4 = iterator_4->next;
		counter++;
	}

	return 0;
}

unsigned int list_size(struct List *list) {
	/*
	 * This function is a copy & paste of search, but it doesn't look for a value, instead, it will just print the list size in the end
	 */

	struct Node* iterator_5;
	iterator_5 = list->head;
	unsigned int counter = 1;

	while (1) {
		//End of the List
		if (iterator_5 == list->tail) {
			printf("Size: %i\n",counter);
			return 0;
		}
		iterator_5 = iterator_5->next;
		counter++;
	}

	return 0;
}


struct List *create_sorted_list(long* value_array, unsigned int array_lenght) {
	/*
	 * This function creates a new, already sorted list by creating a new one and then inserting the values in order.
	 */

	struct List *new_List; //Creates a new List
	new_List = (struct List*) malloc(sizeof(struct List)); //Allocates the memory
	new_List->head=0;
	new_List->tail=0;

	long* iterator; //Iterator used for pointer arithmetics

	for(iterator = value_array;iterator<(value_array+array_lenght);iterator++){
		insert_value(new_List, *iterator);
	}

	printf("List created!\n");

	return new_List;
}
void print_list(struct List *list){
	/*
	 * This function prints the data values of the list in order.
	 */

	//Required for navigating through the list
	struct Node* iterator_6;

	//Prints out the head
	printf("%ld ",list->head->data);

	//In the beginning, the iterator is the next Node after the head
	iterator_6 = list->head->next;

	while(1){
		printf("%ld ",iterator_6->data);

		//If the next node is the tail, print it and exit
		if(iterator_6 -> next == list->tail){
			printf("%ld\n",list->tail->data);
			printf("Done!\n");
			return;
		}
		//Else just get the next node as the current one
		iterator_6 = iterator_6->next;
	}
}

void print_reverse(struct List *list){
	/*
	 * This function prints the data values of the list in reverse order.
	 */

	//Required for navigating through the list
	struct Node* iterator_7;

	//Prints out the tail
	printf("%ld ",list->tail->data);

	//In the beginning, current is the next Node before the tail
	iterator_7 = list->head->next;

	while(1){
		printf("%ld ",iterator_7->data);

		//If the previous node is the head, print it and exit
		if(iterator_7->previous == list->head){
			printf("%ld\n",list->head->data);
			printf("Done!\n");
			return;
		}
		//Else just get the next node as the current one
		iterator_7 = iterator_7->previous;
	}
}

