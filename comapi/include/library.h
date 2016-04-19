#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//
// Connection
//

typedef enum { // TODO: Agregar CONNECTION_PIPE_FILE / CONNECTION_PIPE_FD?
	CONNECTION_PIPE = 0,
	CONNECTION_SOCKET
} ctype_t;

typedef union {
	char * path;
	char * ip;
} address_t;

typedef union {
	int fd;
	int port;
} channel_t;

typedef struct {
	ctype_t type;
	address_t address;
	channel_t channel;
} __attribute__((packed)) connection_t;

//
// General
//

typedef enum {
	VALUE_BOOLEAN = 0,
	VALUE_INTEGER,
	VALUE_STRING
} vtype_t;

typedef union {
	int  ;
	int integer;
	char * string;
} value_t;

typedef struct {
	vtype_t type;
	value_t value;
} __attribute__((packed)) data_t;

typedef response_t * (*handler_t)(request_t *);

//
// Request
//

typedef enum {
	ACTION_LIST = 0,
	ACTION_BET,
	ACTION_MONEY_LEFT,
	ACTION_MONEY_RESET,
	ACTION_DISCONNECT // TODO: ?
} action_t;

typedef struct {
	action_t action;
	int argc;
	data_t * argv;
} __attribute__((packed)) request_t;

//
// Response
//

typedef enum {
	STATUS_WAITING = 0,
	STATUS_WORKING,
	STATUS_DONE
} status_t; // TODO: Necesario?

typedef struct {
	status_t status; // TODO: Necesario?
	int argc;
	data_t * argv;
} __attribute__((packed)) response_t;

connection_t * connection_new();
void connection_free(connection_t * connection);

#endif
