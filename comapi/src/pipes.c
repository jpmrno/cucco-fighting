#include <pipes.h>

#define FIFO_MODE 0666

pipe_t * pipe_make(char * address, int write) {
	FILE * file;
	pipe_t * pipe;

	if(mkfifo(address, FIFO_MODE) == -1) {
		return NULL;
	}

	pipe = pipe_open(address, write);
	if(pipe == NULL) {
		unlink(address);
		return NULL;
	}

	pipe->owner = TRUE;

	return pipe;
}

pipe_t * pipe_open(char * address, int write) {
	FILE * file;
	pipe_t * pipe;

	file = fopen(address, write ? "w" : "r");
	if(file == NULL) {
		return NULL;
	}

	pipe = malloc(sizeof(pipe_t));
	if(pipe == NULL) {
		fclose(file);
		return NULL;
	}

	pipe->address = malloc(sizeof(char) * strlen(address));
	if(pipe->address == NULL) {
		fclose(file);
		free(pipe);
		return NULL;
	}

	pipe->owner = FALSE;
	strcpy(pipe->address, address);
	pipe->channel = file;

	return pipe;
}

void pipe_remove(pipe_t * pipe) {
	if(pipe != NULL) {
		if(pipe->owner) {
			unlink(pipe->address);
		}
		fclose(pipe->channel);
		free(pipe->address);
		free(pipe);
	}
}
