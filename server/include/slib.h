#ifndef _SERVER_LIB_H_
#define _SERVER_LIB_H_

#include <comapi.h>

int opcode(connection_t connection);

int login(connection_t connection);
int money(connection_t connection);
int cucco_add(connection_t connection);
int cucco_remove(connection_t connection);
int bet(connection_t connection);
int reset(connection_t connection);
int logout(connection_t connection);

#endif
