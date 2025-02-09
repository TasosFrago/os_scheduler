#ifndef METRICS_H
#define METRICS_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	int32_t pid;
	int32_t waiting_time;
	int32_t completion_time;
	int32_t turnaround_time;
	int32_t context_switches;
	int32_t response_time;
	int32_t first_execution;
	int32_t arrival;
	int32_t burst;
} ProcMetrics;

typedef struct {
	ProcMetrics *elems;
	size_t size;
} Metrics_arr;

Metrics_arr *metrArr_new(size_t size);
void metrArr_destroy(Metrics_arr *arr);

size_t metrArr_len(Metrics_arr *arr);
int metrArr_append(Metrics_arr *arr, ProcMetrics elem);
void metrArr_set(Metrics_arr *arr, ProcMetrics elem, size_t index);
ProcMetrics *metrArr_get(Metrics_arr *arr, size_t index);

void metrArr_calculateMetrics(Metrics_arr *arr);
void metrArr_pprint(Metrics_arr *arr);

#endif // METRICS_H
