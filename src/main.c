#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "lib/pcbVec.h"
#include "lib/queue.h"
#include "lib/metrics.h"
#include "lib/processJSONParser.h"

#include "SRTF.h"
#include "RoundRobin.h"
#include "FCFS.h"
#include "Priority_Preemptive.h"


typedef enum {
	FCFS,
	PRIORITY_PREEMPTIVE,
	SRTF,
	ROUND_ROBIN
} algrithm_t;

void printProcess(PCB_vec *vec);
int calculateMaxTime(PCB_vec *vec);


int main(int argc, char *argv[])
{
	if(argc < 3) {
		printf("Not enough arguments given\n");
		printf("./main [path/to/file-with-processes.json] [algorithm-to-run] [optional:spefify-quantum-for-round-robin]\n");
		return -1;
	}

	PCB_vec *processes = pcbVec_new();
	int err = read_JSON_ProcessF(argv[1], &processes);
	if(err < 0) {
		goto CLEANUP;
	}
	printProcess(processes);

	algrithm_t algo;
	if(strncmp(argv[2], "fcfs", 4) == 0) {
		printf("Using FCFS algorithm\n");
		algo = FCFS;
	} else if(strncmp(argv[2], "priority-preemptive", 20) == 0) {
		printf("Using Priority Preemptive algorithm\n");
		algo = PRIORITY_PREEMPTIVE;
	} else if(strncmp(argv[2], "srtf", 4) == 0) {
		printf("Using SRTF algorithm\n");
		algo = SRTF;
	} else if(strncmp(argv[2], "round-robin", 11) == 0) {
		printf("Using Round Robin algorithm\n");
		algo = ROUND_ROBIN;
	} else {
		printf("Algorithm not defined correctly.\n Available alglrithms: fcfs, priority-preemptive, srtf, round-robin\n");
		goto CLEANUP;
	}

	int quantum = -1;
	if(argc == 4 && algo == ROUND_ROBIN) {
		quantum = atoi(argv[3]);
		if(quantum <= 0) {
			printf("Invalid quantum given. Will use default\n");
			quantum = -1;
		}
	}

	Metrics_arr *metrics = metrArr_new(pcbVec_length(processes));
	// Fill metrics array with pids
	for(int i = 0; i < pcbVec_length(processes); i++) {
		// Initialize rest of the values with -1 to be able to track if they changed
		metrArr_set(metrics, (ProcMetrics){
			.pid = i+1,
			.waiting_time = 0,
			.completion_time = 0,
			.turnaround_time = 0,
			.context_switches = -1, // Start from -1 so the first run does not count as a context switch
			.response_time = 0,
			.first_execution = -1,
			.arrival = (int32_t)pcbVec_get(processes, i)->arrival,
			.burst = (int32_t)pcbVec_get(processes, i)->burst,
		}, i);
	}

	QueueInt_h ready_q, done_q, gantt_q;
	q_new(&ready_q);
	q_new(&done_q);
	q_new(&gantt_q);

	int running_pid = 0, prev_running = 0;
	int current_quantum = 0;

	for(int time = 0; time < calculateMaxTime(processes); time++) {
		printf("=== Simulation time: %d ===\n", time);
		for(int i = 0; i < pcbVec_length(processes); i++) {
			PCB *proc = pcbVec_get(processes, i);
			if(proc->arrival == time) {
				q_push(&ready_q, proc->pid);
				printf("\tIncoming process: %d\n", proc->pid);
			}
		}

		prev_running = running_pid;

		switch (algo) {
			case FCFS:
				fcfs(&ready_q, &running_pid, processes);
				break;
			case PRIORITY_PREEMPTIVE:
				priority_preemptive(&ready_q, &running_pid, processes);
				break;
			case SRTF:
				srtf(&ready_q, &running_pid, processes);
				break;
			case ROUND_ROBIN:
				rr(&ready_q, &running_pid, processes, &current_quantum, quantum);
				break;
		}

		q_push(&gantt_q, running_pid);

		if(running_pid != 0) {
			PCB *running_proc = pcbVec_get(processes, running_pid-1);
			ProcMetrics *running_proc_metr = metrArr_get(metrics, running_pid-1);
			running_proc->remaining--;
			current_quantum++;

			if(running_pid != prev_running) {
				metrArr_get(metrics, running_pid - 1)->context_switches++;
			}

			//==============
			printf("\tRunning process pid: %d\n", running_pid);
			printf("\t\t| reamining: %d\n", running_proc->remaining);
			printf("\tReady Queue: [ ");

			foreach_nodeR(ready_q, elem) {
				printf("%d", elem->data);
				if(elem->prev != NULL) {
					printf(", ");
				}
			}
			printf(" ]\n");

			printf("\tDone Queue: [ ");
			foreach_nodeR(done_q, elem) {
				printf("%d", elem->data);
				if(elem->prev != NULL) {
					printf(", ");
				}
			}
			printf(" ]\n\n");
			//==============

			// Check if process finished
			if(running_proc->remaining <= 0) {
				q_push(&done_q, running_pid); // Add to process to the DONE queue
				running_proc_metr->completion_time = time + 1; // +1 to account for this quantum of time
				running_pid = 0;
			}
			// Set first execution time
			if(running_proc_metr->first_execution < 0) {
				running_proc_metr->first_execution = time;
			}
		}
	}
	printf("\nFinished:\n");
	printf("\tFinal Done Queue: [ ");
	foreach_nodeR(done_q, elem) {
		printf("%d", elem->data);
		if(elem->prev != NULL) {
			printf(", ");
		}
	}
	printf(" ]\n");

	printf("\tGant Graph:\n");
	printf("\t\tTime: | ");
	for(int i = 0; i < calculateMaxTime(processes); i++) {
		printf("%-2d | ", i);
	}
	printf("\n\t\tPP:   | ");
	foreach_nodeR(gantt_q, elem) {
		printf("P%-2d| ", elem->data);
	}
	printf(" |\n\n");

	q_destroy(&ready_q);
	q_destroy(&done_q);
	q_destroy(&gantt_q);

	metrArr_calculateMetrics(metrics);

	/*printProcess(processes);*/
	metrArr_pprint(metrics);

	metrArr_destroy(metrics);

CLEANUP:
	pcbVec_destroy(processes);

	return 0;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
/* Calculates the maximum time the processes in vec could run so we know how much to run the simulation */
int calculateMaxTime(PCB_vec *vec)
{
	int sum_burst = 0;
	PCB *latest_proc = pcbVec_get(vec, 0);
	for(int i = 0; i < pcbVec_length(vec); i++) {
		PCB *tmp = pcbVec_get(vec, i);
		sum_burst += tmp->burst;
		if(tmp->arrival > latest_proc->arrival) {
			latest_proc = tmp;
		} else if(tmp->arrival == latest_proc->arrival && tmp->burst > latest_proc->burst) {
			latest_proc = tmp;
		}
	}
	return MAX(sum_burst, (latest_proc->arrival + latest_proc->burst));
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
