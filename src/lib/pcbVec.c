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

int pcbVec_new(PCB_vec **vec)
{
	*vec = (PCB_vec *)malloc(sizeof(PCB_vec *) + PCB_VEC_SIZE * sizeof(PCB));
	(*vec)->size = 0;
	(*vec)->capacity = PCB_VEC_SIZE;
	(*vec)->processes = (PCB *)((*vec) + 1);
	return 0;
}

void pcbVec_destroy(PCB_vec *vec)
{
	free(vec);
}

size_t pcbVec_length(PCB_vec *vec)
{
	return vec->size;
}

int pcbVec_append(PCB_vec *vec, PCB pcb)
{
	if(vec == NULL) {
		return -1;
	}
	if(vec->size >= vec->capacity) {
		if(vec->capacity < 100) vec->capacity *= 2;
		else vec->capacity += 50;
		vec->processes = realloc(vec->processes, (vec->capacity) * sizeof(PCB));
	}
	vec->processes[vec->size] = pcb;
	++vec->size;
	return 0;
}

PCB pcbVec_get(PCB_vec *vec, int index)
{
	assert(index < vec->size && "Index out of bounds");
	return vec->processes[index];
}
