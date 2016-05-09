#ifndef _COMAPI_H_
#define _COMAPI_H_

#include <unistd.h>

// ---[ Connection ]-----------------------------------
struct network_t;
typedef struct network_t * connection_t;
// ----------------------------------------------------

#define SERVER // TODO: Remove
#define CLIENT // TODO: Remove
#define OK_DEFINED // TODO: Remove

// ---[ Server ]---------------------------------------
#ifdef SERVER
// #define OK_DEFINED
#ifdef CLIENT
// TODO: Error de compilacion?
#endif

connection_t server_open(const char * config_file);
void server_close(connection_t connection);
void server_ajar(connection_t connection);

connection_t server_accept(connection_t connection);
#endif
// ----------------------------------------------------

// ---[ Client ]---------------------------------------
#ifdef CLIENT
// #define OK_DEFINED
#ifdef SERVER
// TODO: Error de compilacion?
#endif

connection_t server_connect(const char * config_file);
void server_disconnect(connection_t connection);
#endif
// ----------------------------------------------------

// ---[ Common ]---------------------------------------
#ifdef OK_DEFINED
int server_send(connection_t connection, const void * data, size_t size);
int server_receive(connection_t connection, void * data, size_t size);
#endif
// ----------------------------------------------------

#endif