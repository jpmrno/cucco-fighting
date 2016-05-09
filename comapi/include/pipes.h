#ifndef _PIPES_H_
#define _PIPES_H_

struct npipe_t;
typedef struct npipe_t * pipe_t;

pipe_t pipe_make(char * address, int write);
void pipe_remove(pipe_t pipe);

pipe_t pipe_open(char * address, int write);
void pipe_close(pipe_t pipe);

int pipe_send(pipe_t pipe, void * data, const int size);
int pipe_receive(pipe_t pipe, void * data, const int size);

#endif
