#ifndef _SERVER_LIB_H_
#define _SERVER_LIB_H_

#include <comapi.h>
#include <smemory.h>

int opcode(connection_t connection);

int login(connection_t connection);
int money(connection_t connection, double wallet);
int cucco_add(connection_t connection, smemory_t database);
int cucco_remove(connection_t connection, smemory_t database);
int list(connection_t connection, smemory_t database);
int bet(connection_t connection, double * wallet, int * clients, int * bettors, char * winner, smemory_t database);
int reset(connection_t connection);

int db_close(smemory_t database);

#endif
