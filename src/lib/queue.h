#ifndef QUEUE_H
#define QUEUE_H

struct Node {
	int data;
	struct Node *next;
	struct Node *prev;
};

typedef struct {
	struct Node *front;
	struct Node *rear;
} QueueInt_t;

typedef QueueInt_t * QueueInt_h;

typedef struct {
	enum {
		VALUE,
		ERR
	} type;
	union {
		int val;
		int empty;
	};
} PopResult;

void q_new(QueueInt_h *q);
void q_destroy(QueueInt_h *q);
void q_push(QueueInt_h *q, int val);
PopResult q_pop(QueueInt_h *q);

#endif // QUEUE_H
