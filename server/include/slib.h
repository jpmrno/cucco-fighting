#ifndef _SERVER_LIB_H_
#define _SERVER_LIB_H_

#include <comapi.h>

int login(connection_t connection);
int add_cucco(connection_t connection);
int remove_cucco(connection_t* connection);
int bet(connection_t* connection);
int reset(connection_t* connection);
int money(connection_t* connection);


#endif
