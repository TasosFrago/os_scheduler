#include <stdio.h>
#include <stdint.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "lib/pcbVec.h"
#include "lib/queue.h"
#include "lib/processJSONParser.h"

#include "SRTF.h"
#include "RoundRobin.h"
#include "FCFS.h"
#include "Priority_Preemptive.h"

void printProcess(PCB_vec *vec);

int q_removeByPID(QueueInt_h *q, int pid);

int main()
{
	PCB_vec *processes = pcbVec_new();

	read_JSON_ProcessF("processes/01_processes.json", &processes);

	QueueInt_h ready_q, done_q;
	q_new(&ready_q);
	q_new(&done_q);

	int running_pid = 0;
	int current_quantum = 0;

	for(int time = 0; time < 20; time++) {
		printf("=== Simulation time: %d ===\n", time);
		for(int i = 0; i < pcbVec_length(processes); i++) {
			PCB *proc = pcbVec_get(processes, i);
			if(proc->arrival == time) {
				q_push(&ready_q, proc->pid);
				printf("\tIncoming process: %d\n", proc->pid);
			}
		}

		/*srtf(&ready_q, &running_pid, processes);*/
		/*rr(&ready_q, &running_pid, processes, &current_quantum);*/
		/*fcfs(&ready_q, &running_pid, processes);*/
		priority_preemptive(&ready_q, &running_pid, processes);

		if(running_pid != 0) {
			PCB *running_proc = pcbVec_get(processes, running_pid-1);
			running_proc->remaining--;
			current_quantum++;

			printf("\tRunning process pid: %d\n", running_pid);
			printf("\t\t| reamining: %d\n", running_proc->remaining);
			printf("\tReady Queue: [ ");

			foreach_node(ready_q, elem) {
				printf("%d, ", elem->data);
			}
			printf("]\n");

			printf("\tDone Queue: [ ");

			foreach_node(done_q, elem) {
				printf("%d, ", elem->data);
			}
			printf("]\n\n");

			if(running_proc->remaining <= 0) {
				q_push(&done_q, running_pid); // Add to process to the DONE queue
				running_pid = 0;
			}
		}

	}
	printf("\nFinished:");
	printf("\tFinal Done Queue: [ ");
	foreach_node(done_q, elem) {
		printf("%d, ", elem->data);
	}
	printf("]\n");

	q_destroy(&ready_q);
	q_destroy(&done_q);

	printProcess(processes);

	pcbVec_destroy(processes);

	return 0;
}

int q_removeByPID(QueueInt_h *q, int pid)
{
	if(q == NULL || *q == NULL || (*q)->rear == NULL)
		return 0;

	foreach_node(*q, elem) {
		if(elem->data == pid) {
			q_remove(q, elem);
			return 1;
		}
	}
	return 0;
}

void printProcess(PCB_vec *vec)
{
	for(int i = 0; i < pcbVec_length(vec); i++) {
		printf("Process %d:\n", i+1);
		printf("\tPID: %d\n", pcbVec_get(vec, i)->pid);
		printf("\tarrival: %d\n", pcbVec_get(vec, i)->arrival);
		printf("\tburst: %d\n", pcbVec_get(vec, i)->burst);
		printf("\tremaining: %d\n", pcbVec_get(vec, i)->remaining);
		printf("\tpriority: %d\n", pcbVec_get(vec, i)->priority);
		printf("\n");
	}
}
