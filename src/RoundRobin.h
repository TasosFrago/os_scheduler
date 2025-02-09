#include "lib/pcbVec.h"
#include "lib/queue.h"


#define ROUND_ROBIN_QUANTUM 1
void rr(QueueInt_h *ready_q, int *running_pid, PCB_vec *processes, int *current_quantum)
{
	if(*running_pid == 0) {
		PopResult res = q_pop(ready_q);
		if(res.type != ERR) {
			*running_pid = res.val;
			*current_quantum = 0;
		}
	} else if(*current_quantum >= ROUND_ROBIN_QUANTUM) {
		q_push(ready_q, *running_pid);
		*running_pid = q_pop(ready_q).val;
		*current_quantum = 0;
	}
}
