#ifndef _COMAPI_H_
#define _COMAPI_H_

connection_t * mkconnection(ctype_t type, address_t address);
int rmconnection(connection_t * connection);
// connection_t * connect(atype_t type, address_t address);
// response_t * request(connection_t * connection, request_t * request);
// void listen(connection_t * connection, handler_t handler);

#endif