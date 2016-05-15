#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include <define.h>
#include <sys/types.h>

#define MESSAGE_SIZE_MAX 128

typedef struct {
	long type;
	level_t level;
	char text[MESSAGE_SIZE_MAX];
} qmessage_t;

struct mq_t;
typedef struct mq_t * mqueue_t;

mqueue_t mq_make(key_t key);
void mq_remove(mqueue_t mqueue);

mqueue_t mq_open(key_t key);
void mq_close(mqueue_t mqueue) ;

int mq_send(mqueue_t mqueue, qmessage_t * message);
int mq_receive(mqueue_t mqueue, long type, qmessage_t * message);
int peek_peek(mqueue_t mqueue, long type);

#endif
