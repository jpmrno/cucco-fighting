#ifndef _DATABASE_H_
#define _DATABASE_H_

int database_open();
void database_close();

int database_cuccos_add(char * name);
int database_cuccos_get(char * name);
int database_cuccos_getn(int number);
int database_cuccos_remove(char * name);
int database_cuccos_removen(int number);
int database_cuccos_list();

int database_clients_add(char * name);
int database_clients_get(char * name);
int database_clients_getn(int number);
int database_clients_remove(char * name);
int database_clients_removen(int number);
int database_clients_list();
int database_clients_money(char * name, int money);
int database_clients_moneyn(int id, int money);

#endif
