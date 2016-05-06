#ifndef _COMAPI_H_
#define _COMAPI_H_

//
// Connection
//

struct network_t;
typedef struct network_t * connection_t;

// //
// // General
// //

// typedef enum {
// 	VALUE_BOOLEAN = 0,
// 	VALUE_INTEGER,
// 	VALUE_STRING
// } type_t;

// typedef union {
// 	int boolean;
// 	int integer;
// 	char * string;
// } value_t;

// typedef struct {
// 	type_t type;
// 	value_t value;
// } __attribute__((packed)) arg_t;

// typedef struct {
// 	int size;
// 	arg_t * vars;
// } __attribute__((packed)) args_t;

connection_t c_mkserver(char * address, ...);
connection_t c_connect(char * address, ...);
void c_disconnect(connection_t connection);
connection_t c_accept(connection_t connection);

// int send(connection_t connection, data_t * data);
// int receive(connection_t connection, data_t * data);

#endif