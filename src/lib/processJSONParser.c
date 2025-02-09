#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "jsmn/jsmn.h"
#include "lib/pcbVec.h"

#define JSON_CHUNK_SIZE 1024

static size_t read_chunk(FILE *file, char *buffer, size_t buffer_size)
{
	return fread(buffer, 1, buffer_size, file);
}

// For internal debuging
/*static const char *jsmntype_to_string(jsmntype_t type) {*/
/*	switch (type) {*/
/*		case JSMN_UNDEFINED: return "JSMN_UNDEFINED";*/
/*		case JSMN_OBJECT:    return "JSMN_OBJECT";*/
/*		case JSMN_ARRAY:     return "JSMN_ARRAY";*/
/*		case JSMN_STRING:    return "JSMN_STRING";*/
/*		case JSMN_PRIMITIVE: return "JSMN_PRIMITIVE";*/
/*		default:             return "UNKNOWN";*/
/*	}*/
/*}*/

#define PARSE_jsonEQ(str, index) \
	(strncmp((json_chunk+tokens[(index)].start), (str), (tokens[index].end - tokens[index].start)) == 0)

static int parse_json_chunk(const char *json_chunk, size_t chunk_size, PCB_vec *procVec, jsmn_parser *p, jsmntok_t *tokens)
{
	int num_tokens = jsmn_parse(p, json_chunk, chunk_size, tokens, chunk_size);
	if(num_tokens < 0) {
		printf("Failed to parse JSON chunk.\n");
		return -1;
	}

	for(int i = 0; i < num_tokens; i++) {
		if(tokens[i].type == JSMN_OBJECT) {
			PCB newProcess = {};
			for(int j = 1; j < (1+4)*2; j += 2) {
				int idx = i+j; // index to get each line of the object

				if(PARSE_jsonEQ("pid", idx)) {
					newProcess.pid = atoi(json_chunk+tokens[idx+1].start);

				} else if(PARSE_jsonEQ("arrival", idx)) {
					newProcess.arrival = atoi(json_chunk+tokens[idx+1].start);

				} else if(PARSE_jsonEQ("burst", idx)) {
					newProcess.burst = atoi(json_chunk+tokens[idx+1].start);
					// remaining same as burst at start
					newProcess.remaining = atoi(json_chunk+tokens[idx+1].start);

				} else if(PARSE_jsonEQ("priority", idx)) {
					/*processes[*process_index].priority = atoi(json_chunk+tokens[idx+1].start);*/
					newProcess.priority = atoi(json_chunk+tokens[idx+1].start);

				}
			}
			pcbVec_append(procVec, newProcess);
		}
	}
	return pcbVec_length(procVec);
}

int read_JSON_ProcessF(const char *filename, PCB_vec **processes)
{
	FILE *f = fopen(filename, "r");
	if(!f) {
		printf("Error opening file\n");
		return -1;
	}

	int num_proc = 0;
	jsmn_parser p;
	jsmntok_t tokens[500];
	jsmn_init(&p);

	char chunk[JSON_CHUNK_SIZE];
	size_t chunk_size;

	if((chunk_size = read_chunk(f, chunk, JSON_CHUNK_SIZE)) > 0) {
		num_proc = parse_json_chunk(chunk, chunk_size, *processes, &p, tokens);
	} else {
		return -2;
	}
	fclose(f);
	return num_proc;
}
