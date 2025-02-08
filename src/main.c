#include <stdio.h>
#include <stdint.h>

#include <stddef.h>
#include <assert.h>

#include "lib/pcbVec.h"
#include "lib/processJSONParser.h"
#include "lib/queue.h"


void printProcess(PCB_vec *vec);


int main()
{
	PCB_vec *processes = pcbVec_new();

	QueueInt_h Q;
	q_new(&Q);
	q_push(&Q, 1);
	q_push(&Q, 2);
	q_push(&Q, 3);
	q_push(&Q, 4);

	PopResult tmp; 
	do {
		tmp = q_pop(&Q);
		if(tmp.type == ERR) {
			printf("Empty queue\n");
		} else {
			printf("el: %d\n", tmp.val);
		}
	} while(tmp.type != ERR);

	q_destroy(&Q);

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
