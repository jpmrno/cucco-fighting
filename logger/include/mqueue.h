#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include <sys/types.h>

struct mq_t;
typedef struct mq_t * mqueue_t;

mqueue_t mq_make(key_t key);
void mq_remove(mqueue_t mqueue);
mqueue_t mq_open(key_t key);
void mq_close(mqueue_t mqueue);

#endif