#include <stdio.h>
//------------------ 

#include "lib/queue.h"
#include "lib/pcbVec.h"

void fcfs(QueueInt_h *ready_q, int *running_pid, PCB_vec *processes) 
{
    if (q_isEmpty(ready_q)) return;  

    if (*running_pid == 0) {  
        PopResult result = q_pop(ready_q);  
        if (result.type == VALUE) {  
            *running_pid = result.val;  
        }
    }
}







/*


void fcfs(QueueInt_h job_q, QueueInt_h ready_q, QueueInt_h waiting_q, PCB_vec **processes) {
    int timeArrival_pop[4];
    int pid_sorted[4];
    int burst[4];
    int remaining[4];
    int priority[4];
    int temp;

    
    for (int i = 0; i < 4; i++) {
        timeArrival_pop[i] = pcbVec_get(processes, i).arrival;
        pid_sorted[i] = pcbVec_get(processes, i).pid;
        burst[i] = pcbVec_get(processes, i).burst;
        remaining[i] = pcbVec_get(processes, i).remaining;
        priority[i] = pcbVec_get(processes, i).priority;
    }

    // Bubble Sort για ταξινόμηση με βάση τους χρόνους άφιξης (arrival)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4 - i - 1; j++) {
            if (timeArrival_pop[j] > timeArrival_pop[j + 1]) {
                // Ανταλλαγή θέσεων με χρήση της μεταβλητής temp
                temp = timeArrival_pop[j];
                timeArrival_pop[j] = timeArrival_pop[j + 1];
                timeArrival_pop[j + 1] = temp;

                temp = pid_sorted[j];
                pid_sorted[j] = pid_sorted[j + 1];
                pid_sorted[j + 1] = temp;

                temp = burst[j];
                burst[j] = burst[j + 1];
                burst[j + 1] = temp;

                temp = remaining[j];
                remaining[j] = remaining[j + 1];
                remaining[j + 1] = temp;

                temp = priority[j];
                priority[j] = priority[j + 1];
                priority[j + 1] = temp;
            }
        }
    }

   
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < burst[i]; j++) {
            q_push(&job_q, pid_sorted[i]);
            q_push(&ready_q, &job_q);
            printf("Process %d pushed to ready queue (burst time %d)\n", pid_sorted[i], burst[i]);
        }
    }

   
}

*/