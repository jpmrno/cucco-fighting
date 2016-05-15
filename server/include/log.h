#ifndef _LOG_H_
#define _LOG_H_

#include <define.h>

int log_open();
int log_send(level_t level, char * message);
void log_close();

#endif