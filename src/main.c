#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "lib/pcbVec.h"
#include "lib/processJSONParser.h"


void printProcess(PCB_vec *vec);

#define QUEUE_INIT_SIZE 16

struct Node {
	int data;
	struct Node *next;
	struct Node *prev;
};

struct Node *node_new(int val)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	assert(newNode != NULL && "Failed to allocate new Node");
	newNode->data = val;
	newNode->next = newNode->prev = NULL;
	return newNode;
}

typedef struct {
	struct Node *front;
	struct Node *rear;
} QueueInt_t;

typedef QueueInt_t * QueueInt_h;

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

int q_pop(QueueInt_h *q)
{
	assert(q != NULL && "Can't pop from unallocated queue");
	struct Node *tmp = (*q)->front;
	int data = tmp->data;

	(*q)->front = (*q)->front->prev;
	if((*q)->front != NULL) {
		(*q)->front->next = NULL;
	} else {
		(*q)->rear = NULL; // when poping last element make rear null to avoid dangling pointers
	}
	free(tmp);
	return data;
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

/*typedef struct {*/
/*	size_t size;*/
/*	size_t capacity;*/
/*	int *elements;*/
/*} QueueInt;*/
/**/
/*typedef QueueInt * QueueI_t;*/
/**/
/*void q_new(QueueI_t *q)*/
/*{*/
/*	*q = (QueueI_t)malloc(sizeof(QueueInt) + QUEUE_INIT_SIZE * sizeof(int));*/
/*	assert(q != NULL && "Failed to allocate queue");*/
/*	(*q)->size = 0;*/
/*	(*q)->capacity = QUEUE_INIT_SIZE;*/
/*	(*q)->elements = (int *)((*q) + 1);*/
/*}*/
/**/
/*void q_destroy(QueueI_t *q)*/
/*{*/
/*	free(q);*/
/*}*/
/**/
/*void q_push(QueueI_t *q, int item)*/
/*{*/
/*	QueueI_t Q = *q;*/
/*	assert(q != NULL && "Can't push on empty array");*/
/*	assert(Q->elements != NULL && "Can't push on unallocated array");*/
/**/
/*	if((*q)->size == (*q)->capacity) {*/
/*		(*q)->capacity *= 2;*/
/**/
/*		*q = realloc(*q, sizeof(QueueInt) + (*q)->capacity * sizeof(int));*/
/*		assert(q != NULL && "Failed to reallocate queue");*/
/**/
/*		(*q)->elements = (int *)((*q) + 1);*/
/*	}*/
/*	(*q)->elements[(*q)->size] = item;*/
/*	++(*q)->size;*/
/*}*/
/**/
/*int q_pop(QueueI_t *q)*/
/*{*/
/*	assert(q != NULL && "");*/
/*	assert((*q)->elements != NULL && "Can't pop from unallocated array");*/
/*	return 0;*/
/*}*/
/**/
/*/**/
/* * Queue - Get `index` element of the queue without poping it from the queue*/
/* */
/*int q_get(QueueI_t *q, size_t index)*/
/*{*/
/*	assert(q != NULL && "Can't get from unallocated queue");*/
/*	assert(index <= (*q)->size && "Queue index out of bounds");*/
/*	return (*q)->elements[index];*/
/*}*/
/**/
/*size_t q_len(QueueI_t *q)*/
/*{*/
/*	assert(q != NULL && "Unallocated queue");*/
/*	return (*q)->size;*/
/*}*/
/**/

int main()
{
	PCB_vec *processes = pcbVec_new();

	QueueInt_h Q;
	q_new(&Q);
	q_push(&Q, 1);
	q_push(&Q, 2);
	q_push(&Q, 3);
	q_push(&Q, 4);

	printf("el: %d\n", q_pop(&Q));
	printf("el: %d\n", q_pop(&Q));
	printf("el: %d\n", q_pop(&Q));
	printf("el: %d\n", q_pop(&Q));

	printf("el: %d\n", q_pop(&Q));

	q_destroy(&Q);
	/*QueueI_t Q;*/
	/*q_new(&Q);*/
	/*q_push(&Q, 1);*/
	/*for(int i = 0; i < 20; i++) {*/
	/*	q_push(&Q, i);*/
	/*}*/
	/*for(int i = 0; i < 20; i++) {*/
	/*	printf("elem: %d\n", q_get(&Q, i));*/
	/*}*/
	/*printf("size: %lu\n", Q->size);*/

	read_JSON_ProcessF("processes/01_processes.json", &processes);

	printf("Parsed %lu processes:\n", pcbVec_length(processes));
	printProcess(processes);

	pcbVec_destroy(processes);

	return 0;
}

void printProcess(PCB_vec *vec)
{
	for(int i = 0; i < pcbVec_length(vec); i++) {
		printf("Process %d:\n", i+1);
		printf("\tPID: %d\n", pcbVec_get(vec, i).pid);
		printf("\tarrival: %d\n", pcbVec_get(vec, i).arrival);
		printf("\tburst: %d\n", pcbVec_get(vec, i).burst);
		printf("\tremaining: %d\n", pcbVec_get(vec, i).remaining);
		printf("\tpriority: %d\n", pcbVec_get(vec, i).priority);
		printf("\n");
	}
}
