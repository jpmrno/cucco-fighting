#ifndef _PIPES_H_
#define _PIPES_H_

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
} __attribute__((packed)) pipe_t;

pipe_t * pipe_make(char * address, int write);
pipe_t * pipe_open(char * address, int write);
void pipe_remove(pipe_t * pipe);

#endif