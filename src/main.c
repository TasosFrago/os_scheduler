#include <stdio.h>
#include <stdint.h>

#include "lib/pcbVec.h"
#include "lib/processJSONParser.h"

int main() 
{
	PCB_vec *processes;
	pcbVec_new(&processes);

	int num_proc = read_JSON_ProcessF("processes/01_processes.json", &processes);
	printf("num_proc: %d", num_proc);

	printf("Parsed %lu processes:\n", pcbVec_length(processes));
	for(int i = 0; i < pcbVec_length(processes); i++) {
		printf("Process %d:\n", i+1);
		printf("\tPID: %d\n", pcbVec_get(processes, i).pid);
		printf("\tarrival: %d\n", pcbVec_get(processes, i).arrival);
		printf("\tburst: %d\n", pcbVec_get(processes, i).burst);
		printf("\tremaining: %d\n", pcbVec_get(processes, i).remaining);
		printf("\tpriority: %d\n", pcbVec_get(processes, i).priority);
		printf("\n");
	}

	pcbVec_destroy(processes);

	return 0;
}
