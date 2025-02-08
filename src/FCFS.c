#include <stdio.h>
//------------------ 
#include "lib/queue.h"
#include "lib/pcbVec.h"



void fcfs(QueueInt_h job_q, QueueInt_h ready_q, QueueInt_h waiting_q, PCB_vec **processes){


    int timeArrival_pop[4];
    int pid_sorted[4];
    int burst[4];
    int remaining[4];
    int priority[4];
    int temp;

    for ( int i = 0; i < 4; i++)
    {
       timeArrival_pop[i] = pcbVec_get(processes, i).arrival;
       pid_sorted[i] =pcbVec_get(processes, i).pid;
       burst[i] =pcbVec_get(processes, i).burst;
       remaining[i] =pcbVec_get(processes, i).remaining;
       priority[i] =pcbVec_get(processes, i).priority;

    }
    
	
	 // Bubble Sort  με βαση τους χρονους arrival  απο το μικροτερο προς το μελαυτερο 
     // timeArrival_pop[0]=μικρο 
     // timeArrival_pop[3]= μεγαλυτερο 
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4 - i -1 ; j++) {
                if (timeArrival_pop[j] > timeArrival_pop[j + 1]) {
                    // Ανταλλαγή θέσεων (swap)
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


     for (int i=0; i<= burst[0]; i++){
        q_push(&job_q, pid_sorted[0]);
        q_push(&ready_q, &job_q);
     }

     for (int i=0; i<= burst[1]; i++){
        q_push(&job_q, pid_sorted[1]);
        q_push(&ready_q, &job_q);
     }

     for (int i=0; i<= burst[2]; i++){
        q_push(&job_q, pid_sorted[2]);
        q_push(&ready_q, &job_q);
     }

     for (int i=0; i<= burst[3]; i++){
        q_push(&job_q, pid_sorted[3]);
        q_push(&ready_q, &job_q);
     }


}