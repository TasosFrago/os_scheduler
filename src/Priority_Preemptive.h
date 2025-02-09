
#include "lib/queue.h"
#include "lib/pcbVec.h"

void priority_preemptive(QueueInt_h *ready_q, int *running_pid, PCB_vec *processes) 
{
    if (q_isEmpty(ready_q)) return;  

    struct Node *highest_priority_node = (*ready_q)->rear; 
    PCB *highest_priority_proc = pcbVec_get(processes, highest_priority_node->data - 1);

    
    foreach_node((*ready_q), elem) {
        PCB *tmp = pcbVec_get(processes, elem->data - 1);
        if (tmp->priority < highest_priority_proc->priority) {  
            highest_priority_proc = tmp;
            highest_priority_node = elem;
        }
    }

    if (*running_pid == 0) {  
        *running_pid = highest_priority_proc->pid;
        q_remove(ready_q, highest_priority_node);
    } else {
        PCB *running_proc = pcbVec_get(processes, *running_pid - 1);
        if (highest_priority_proc->priority < running_proc->priority) {  
            q_push(ready_q, *running_pid);  
            *running_pid = highest_priority_proc->pid;
            q_remove(ready_q, highest_priority_node);
        }
    }
}