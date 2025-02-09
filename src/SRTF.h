#include "lib/pcbVec.h"
#include "lib/queue.h"


void srtf(QueueInt_h *ready_q, int *running_pid, PCB_vec *processes)
{
	if(q_isEmpty(ready_q)) return;

	struct Node *shortest_pid = (*ready_q)->rear;
	PCB *shortest_proc = pcbVec_get(processes, (*ready_q)->rear->data-1);

	foreach_node((*ready_q), elem) {
		PCB *tmp = pcbVec_get(processes, elem->data - 1);
		if(tmp->remaining < shortest_proc->remaining) {
			shortest_proc = tmp;
			shortest_pid = elem;
		}
	}

	if(*running_pid == 0) {
		*running_pid = shortest_proc->pid;
		q_remove(ready_q, shortest_pid);
	} else if(shortest_proc->remaining < pcbVec_get(processes, *running_pid - 1)->remaining) {
		q_push(ready_q, *running_pid);
		*running_pid = shortest_proc->pid;
		q_remove(ready_q, shortest_pid);
	}
}
