#include <pipes.h>
#include <define.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define FIFO_MODE 0777

// typedef enum {
// 	FILE = 0,
// 	FILE_DESCRIPTOR
// } type_t;

// typedef union {
// 	FILE * file;
// 	int fd;
// } channel_t;

typedef struct {
	int owner;
	char * address;
	int channel;
} __attribute__((packed)) npipe_t;

pipe_t pipe_make(char * address, int write) {
	npipe_t * pipe;

	if(mkfifo(address, FIFO_MODE) == -1) {
		return NULL;
	}

	pipe = (npipe_t *) pipe_open(address, write);
	if(pipe == NULL) {
		unlink(address);
		return NULL;
	}

	pipe->owner = TRUE;

	return (pipe_t) pipe;
}

void pipe_remove(pipe_t pipe) {
	npipe_t * npipe = (npipe_t *) pipe;

	if(pipe != NULL) {
		if(npipe->owner) {
			unlink(npipe->address);
		}
		pipe_close(pipe);
	}
}

pipe_t pipe_open(char * address, int write) {
	int file;
	npipe_t * pipe;

	file = open(address, O_RDWR); // TODO: puse O_RDWR como salvedad...
	if(file == -1) {
		return NULL;
	}

	pipe = malloc(sizeof(npipe_t));
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

	return (pipe_t) pipe;
}

void pipe_close(pipe_t pipe) {
	npipe_t * npipe = (npipe_t *) pipe;

	if(npipe != NULL) {
		close(npipe->channel);
		free(npipe->address);
		free(npipe);
	}
}

int pipe_send(pipe_t pipe, const void * data, size_t size) {
	npipe_t * npipe = (npipe_t *) pipe;

	return write(npipe->channel, data, size);
}

int pipe_receive(pipe_t pipe, void * data, size_t size) {
	npipe_t * npipe = (npipe_t *) pipe;

	return read(npipe->channel, data, size);
}
