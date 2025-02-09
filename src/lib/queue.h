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
void q_remove(QueueInt_h *q, struct Node *elem);
int q_isEmpty(QueueInt_h *q);

#define foreach_node(Q, node) \
	for(struct Node *(node) = (Q)->rear; (node) != NULL; (node) = (node)->next)

#define foreach_nodeR(Q, node) \
	for(struct Node *(node) = (Q)->front; (node) != NULL; (node) = (node)->prev)

#endif // QUEUE_H
