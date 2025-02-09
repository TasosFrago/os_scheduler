#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "lib/pcbVec.h"

struct PCB_vec {
	size_t size;
	size_t capacity;
	PCB *processes;
};

PCB_vec *pcbVec_new()
{
	PCB_vec *vec = malloc(sizeof(PCB_vec));
	assert(vec != NULL && "Failed to allocate vec");
	*vec = (PCB_vec){
		.processes = malloc(sizeof(PCB) * PCB_VEC_SIZE),
		.capacity = PCB_VEC_SIZE
	};
	assert(vec->processes != NULL && "Failed to allocate vec elements");
	return vec;
}

void pcbVec_destroy(PCB_vec *vec)
{
	free(vec->processes);
	free(vec);
}

size_t pcbVec_length(PCB_vec *vec)
{
	return vec->size;
}

int pcbVec_append(PCB_vec *vec, PCB pcb)
{
	assert(vec != NULL && "Trying to append to uninitialized vec");
	if(vec->size == vec->capacity) {
		if(vec->capacity < 100) vec->capacity *= 2;
		else vec->capacity += 50;

		PCB *newProcesses = malloc(vec->capacity * sizeof(PCB));
		for(int i = 0; i < vec->size; i++) {
			newProcesses[i] = vec->processes[i];
		}
		free(vec->processes);
		vec->processes = newProcesses;
		/*vec->processes = realloc(vec->processes, (vec->capacity) * sizeof(PCB));*/
	}
	vec->processes[vec->size] = pcb;
	++vec->size;
	return 0;
}

PCB *pcbVec_get(PCB_vec *vec, int index)
{
	assert(index <= vec->size && "Index out of bounds");
	return &vec->processes[index];
}
