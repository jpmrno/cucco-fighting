#ifndef _PIPES_H_
#define _PIPES_H_

#include <unistd.h>

struct npipe_t;
typedef struct npipe_t * pipe_t;

pipe_t pipe_make(char * address, int write);
void pipe_remove(pipe_t pipe);

pipe_t pipe_open(char * address, int write);
void pipe_close(pipe_t pipe);

int pipe_send(pipe_t pipe, const void * data, size_t size);
int pipe_receive(pipe_t pipe, void * data, size_t size);

#endif
