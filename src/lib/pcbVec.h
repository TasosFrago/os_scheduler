#ifndef PCBVEC_H
#define PCBVEC_H

#include <stdint.h>
#include <stddef.h>

#define PCB_VEC_SIZE 9

typedef struct {
	uint32_t pid;
	uint32_t arrival;
	uint32_t burst;
	uint32_t remaining;
	uint32_t priority;
} PCB;

typedef struct PCB_vec PCB_vec;

int pcbVec_new(PCB_vec **vec);
void pcbVec_destroy(PCB_vec *vec);

size_t pcbVec_length(PCB_vec *vec);
int pcbVec_append(PCB_vec *vec, PCB pcb);
PCB pcbVec_get(PCB_vec *vec, int index);

#endif // PCBVEC_H
