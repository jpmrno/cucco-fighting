#ifndef _DBLIB_H_
#define _DBLIB_H_

struct dbsql_t;
typedef struct dbsql_t * database_t;

typedef int (*callback_t)(void *, int, char **, char **);

database_t database_open(const char * path);
void database_close(database_t database);

int database_cuccos_get(database_t database, char * name, callback_t callback, void * value);
int database_cuccos_add(database_t database, char * name);
int database_cuccos_remove(database_t database, char * name);
int database_cuccos_list(database_t database, callback_t callback, void * value);

#endif
