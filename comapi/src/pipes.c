#include <pipes.h>
#include <define.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define FIFO_MODE 0777

pipe_t * pipe_make(char * address, int write) {
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
	int file;
	pipe_t * pipe;

	file = open(address, O_RDWR); // TODO: puse O_RDWR como salvedad
	if(file == -1) {
		return NULL;
	}

	pipe = malloc(sizeof(pipe_t));
	if(pipe == NULL) {
		close(file);
		return NULL;
	}

	pipe->address = malloc(sizeof(char) * strlen(address));
	if(pipe->address == NULL) {
		close(file);
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
		close(pipe->channel);
		free(pipe->address);
		free(pipe);
	}
}

int pipe_send(pipe_t * pipe, void * data, const int size) {
	return write(pipe->channel, data, size);
}

int pipe_receive(pipe_t * pipe, void * data, const int size) {
	return read(pipe->channel, data, size);
}
