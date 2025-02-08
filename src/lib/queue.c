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

