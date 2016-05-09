#ifndef _COMAPI_H_
#define _COMAPI_H_

// ---[ Connection ]-----------------------------------
struct network_t;
typedef struct network_t * connection_t;
// ----------------------------------------------------

// ---[ Server ]---------------------------------------
#ifdef SERVER
#define OK_DEFINED
#ifdef CLIENT
// TODO: Error de compilacion?
#endif

connection_t server_open(char * config_file);
void server_close(connection_t connection);
void server_disconnect(connection_t connection);

connection_t server_accept(connection_t connection);
#endif
// ----------------------------------------------------

// ---[ Client ]---------------------------------------
#ifdef CLIENT
#define OK_DEFINED
#ifdef SERVER
// TODO: Error de compilacion?
#endif

connection_t server_connect(char * config_file);
void server_disconnect(connection_t connection);
#endif
// ----------------------------------------------------

// ---[ Common ]---------------------------------------
#ifdef OK_DEFINED
// int send(connection_t connection, data_t * data);
// int receive(connection_t connection, data_t * data);
#endif
// ----------------------------------------------------

#endif