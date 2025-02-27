#ifndef PCBVEC_H
#define PCBVEC_H

#include <stdint.h>
#include <stddef.h>

#define PCB_VEC_SIZE 16

typedef struct {
	uint32_t pid;		// process ID
	uint32_t arrival;	// arrival time
	uint32_t burst;		// time that process will run
	uint32_t remaining;	// remaining time to finish
	uint32_t priority;	// priority of the process
} PCB;

typedef struct PCB_vec PCB_vec; // Qpaque type for better encapulation

PCB_vec *pcbVec_new();
void pcbVec_destroy(PCB_vec *vec);

size_t pcbVec_length(PCB_vec *vec);
int pcbVec_append(PCB_vec *vec, PCB pcb);
PCB *pcbVec_get(PCB_vec *vec, int index);

#endif // PCBVEC_H
