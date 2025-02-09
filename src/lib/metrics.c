#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/metrics.h"


Metrics_arr *metrArr_new(size_t size)
{
	assert(size > 0);
	Metrics_arr *arr = (Metrics_arr *)malloc(sizeof(Metrics_arr) + sizeof(ProcMetrics) * size);
	assert(arr != NULL);
	arr->size = size;
	arr->elems = (ProcMetrics *)(arr + 1);

	return arr;
}

void metrArr_destroy(Metrics_arr *arr)
{
	free(arr);
}

size_t metrArr_len(Metrics_arr *arr)
{
	return arr->size;
}

int metrArr_append(Metrics_arr *arr, ProcMetrics elem)
{
	assert(arr != NULL);
	size_t s = arr->size++;
	arr->elems[s] = elem;
	return s;
}

void metrArr_set(Metrics_arr *arr, ProcMetrics elem, size_t index)
{
	assert(arr != NULL);
	assert(index <= arr->size);
	arr->elems[index] = elem;
}

ProcMetrics *metrArr_get(Metrics_arr *arr, size_t index)
{
	assert(arr != NULL && index <= arr->size);
	return &arr->elems[index];
}

void metrArr_calculateMetrics(Metrics_arr *arr)
{
	for(int i = 0; i < metrArr_len(arr); i++) {
		ProcMetrics *process_metrics = metrArr_get(arr, i);

		process_metrics->waiting_time = process_metrics->completion_time - (process_metrics->arrival + process_metrics->burst);
		process_metrics->turnaround_time = process_metrics->completion_time - process_metrics->arrival;
		process_metrics->response_time = process_metrics->first_execution - process_metrics->arrival;
	}
}

#define MGet(i) metrArr_get(arr, (i))

void metrArr_pprint(Metrics_arr *arr)
{
	printf(" RESULTS\n");
	printf("+-------+--------------+-----------------+-----------------+-------------------+-----------------+-------------------+\n");
	printf("| %-5s | %-12s | %-15s | %-15s | %-17s | %-15s | %-17s |\n",
		"PID", "Waiting Time", "Completion Time", "Turnaround Time", "Context Switches", "Response Time", "First Execution");
	printf("+-------+--------------+-----------------+-----------------+-------------------+-----------------+-------------------+\n");

	for(int i = 0; i < metrArr_len(arr); i++) {
		const ProcMetrics *procM = metrArr_get(arr, i);

		printf("| %-5d | %-12d | %-15d | %-15d | %-17d | %-15d | %-17d |\n",
			procM->pid, procM->waiting_time, procM->completion_time, procM->turnaround_time, procM->context_switches, procM->response_time, procM->first_execution);
	}
	printf("+-------+--------------+-----------------+-----------------+-------------------+-----------------+-------------------+\n");
}
