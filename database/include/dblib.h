#ifndef _DBLIB_H_
#define _DBLIB_H_

struct dbsql_t;
typedef struct dbsql_t * database_t;

database_t database_open(const char * path);
void database_close(database_t database);

int database_cuccos_add(database_t database, char * name);
int database_cuccos_get(database_t database, char * name);
int database_cuccos_getn(database_t database, int number);
int database_cuccos_remove(database_t database, char * name);
int database_cuccos_removen(database_t database, int number);
int database_cuccos_list(database_t database);

int database_clients_add(database_t database, char * name);
int database_clients_get(database_t database, char * name);
int database_clients_getn(database_t database, int number);
int database_clients_remove(database_t database, char * name);
int database_clients_removen(database_t database, int number);
int database_clients_list(database_t database);
int database_clients_money(database_t database, char * name, int money);
int database_clients_moneyn(database_t database, int id, int money);

#endif
