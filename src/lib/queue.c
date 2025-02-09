#include <assert.h>
#include <stdlib.h>

#include "lib/queue.h"

struct Node *node_new(int val)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	assert(newNode != NULL && "Failed to allocate new Node");
	newNode->data = val;
	newNode->next = newNode->prev = NULL;
	return newNode;
}


void q_new(QueueInt_h *q)
{
	*q = (QueueInt_h)malloc(sizeof(QueueInt_t));
	(*q)->front = (*q)->rear = NULL;
}

void q_push(QueueInt_h *q, int val)
{
	assert(q != NULL && "Can't push to unallocated queue");
	struct Node *newNode = node_new(val);
	if((*q)->rear == NULL && (*q)->front == NULL) {
		(*q)->front = (*q)->rear = newNode;
		return;
	}
	(*q)->rear->prev = newNode;
	newNode->next = (*q)->rear;
	(*q)->rear = newNode;
}

PopResult q_pop(QueueInt_h *q)
{
	assert(q != NULL && "Can't pop from unallocated queue");
	if((*q)->front == NULL) {
		return (PopResult){
			.type = ERR,
			.empty = 1
		};
	}
	struct Node *tmp = (*q)->front;
	int data = tmp->data;

	(*q)->front = (*q)->front->prev;
	if((*q)->front != NULL) {
		(*q)->front->next = NULL;
	} else {
		(*q)->rear = NULL; // when poping last element make rear null to avoid dangling pointers
	}
	free(tmp);

	return (PopResult){
		.type = VALUE,
		.val = data
	};
}

void q_remove(QueueInt_h *q, struct Node *elem)
{
	assert(q != NULL || *q != NULL);
	if(elem->prev == NULL && elem->next == NULL) { // elem is in both rear and front
		(*q)->front = (*q)->rear = NULL;
	} else if(elem->prev == NULL) { // elem is in the Rear
		(*q)->rear = elem->next;
		elem->next->prev = NULL;
	} else if(elem->next == NULL) { // elem is in the front
		(*q)->front = elem->prev;
		elem->prev->next = NULL;
	} else { // elem is in the middle of the queue
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
	}
	free(elem);
}

int q_isEmpty(QueueInt_h *q)
{
	return ((*q)->rear == NULL && (*q)->front == NULL);
}

void q_destroy(QueueInt_h *q)
{
	while((*q)->rear != NULL) {
		struct Node *tmp = (*q)->rear;
		(*q)->rear = (*q)->rear->next;
		/*(*q)->rear->prev = NULL;*/
		free(tmp);
	}
	free(*q);
	q = NULL;
}

