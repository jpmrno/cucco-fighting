#ifndef _PIPES_H_
#define _PIPES_H_

#include <library.h>

connection_t * pipe_make(address_t address);
connection_t * pipe_connect(address_t address);
int pipe_remove(connection_t * connection);
int pipe_disconnect(connection_t * connection);

#endif
