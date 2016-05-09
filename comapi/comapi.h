#ifndef _COMAPI_H_
#define _COMAPI_H_

#include <config.h>
#include <unistd.h>

// ---[ Connection ]-----------------------------------
struct network_t;
typedef struct network_t * connection_t;
// ----------------------------------------------------

// ---[ Server ]---------------------------------------
#ifdef SERVER
#ifndef CLIENT

#define OK_DEFINED

connection_t server_open(const char * config_file);
void server_close(connection_t connection);
void server_ajar(connection_t connection);
connection_t server_accept(connection_t connection);

#endif
#endif
// ----------------------------------------------------

// ---[ Client ]---------------------------------------
#ifdef CLIENT
#ifndef SERVER

#define OK_DEFINED

connection_t server_connect(const char * config_file);
void server_disconnect(connection_t connection);

#endif
#endif
// ----------------------------------------------------

// ---[ Common ]---------------------------------------
#ifdef OK_DEFINED

int server_send(connection_t connection, const void * data, size_t size);
int server_receive(connection_t connection, void * data, size_t size);

#endif
// ----------------------------------------------------

#endif