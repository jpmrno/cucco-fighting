#ifndef _CLIENT_LIB_H_
#define _CLIENT_LIB_H_

int config(char * path);
int connect(char * user);
void disconnect();

double money();
int cucco_add(char * cucco);
int cucco_remove(char * cucco);
int list(char *** list, int * length);
int bet(char * cucco, double money);
int reset();

#endif
