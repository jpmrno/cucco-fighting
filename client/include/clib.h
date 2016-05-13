#ifndef _CLIENT_LIB_H_
#define _CLIENT_LIB_H_

#include <comapi.h>

int login(connection_t connection, char * user);
double money(connection_t connection);
int cucco_add(connection_t connection, char * cucco);
int cucco_remove(connection_t connection, char * cucco);
int list(connection_t connection, char *** list, int * length);
int bet(connection_t connection, char * cucco, double money);
int reset(connection_t connection);
int logout(connection_t connection);

#endif
