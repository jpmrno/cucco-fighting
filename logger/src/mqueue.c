#include <mqueue.h>
#include <define.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define PERMISSIONS 0666
#define FLAGS_MAKE IPC_CREAT | IPC_EXCL
#define FLAGS_OPEN 0

typedef struct {
	int owner;
	key_t key;
	int id;
} mq_t;

static mq_t * mq_new(int owner, key_t key, int id);
static void mq_free(mq_t * mqueue);

mqueue_t mq_make(key_t key) {
	mq_t * mqueue;
	int id;

	id = msgget(key, FLAGS_MAKE | PERMISSIONS);
	if(id == -1) {
		return NULL;
	}

	mqueue = mq_new(TRUE, key, id);
	if(mqueue == NULL) {
		msgctl(id, IPC_RMID, 0);
		return NULL;
	}

	return (mqueue_t) mqueue;
}

void mq_remove(mqueue_t mqueue) {
	mq_t * mq = (mq_t *) mqueue;

	if(mq != NULL) {
		if(mq->owner) {
			msgctl(mq->id, IPC_RMID, 0);
		}
		mq_close(mqueue);
	}
}

mqueue_t mq_open(key_t key) {
	mq_t * mqueue;
	int id;

	id = msgget(key, FLAGS_OPEN | PERMISSIONS);
	if(id == -1) {
		return NULL;
	}

	mqueue = mq_new(FALSE, key, id);
	if(mqueue == NULL) {
		return NULL;
	}

	return (mqueue_t) mqueue;
}

void mq_close(mqueue_t mqueue) {
	mq_t * mq = (mq_t *) mqueue;

	mq_free(mq);
}

int mq_send(mqueue_t mqueue, message_t * message) {
	mq_t * mq = (mq_t *) mqueue;

	return msgsnd(mq->id, message, sizeof(message_t) - sizeof(long), IPC_NOWAIT);
}

int mq_receive(mqueue_t mqueue, long type, message_t * message) {
	mq_t * mq = (mq_t *) mqueue;

	return msgrcv(mq->id, message, sizeof(message_t) - sizeof(long), type, 0);
}

int peek_peek(mqueue_t mqueue, long type) {
	mq_t * mq = (mq_t *) mqueue;
	int ret;

	ret = msgrcv(mq->id, NULL, 0, type, IPC_NOWAIT);

	return (ret == -1) ? ((errno == E2BIG) ? TRUE : FALSE) : FALSE;
}

static mq_t * mq_new(int owner, key_t key, int id) {
	mq_t * mqueue;

	mqueue = malloc(sizeof(mqueue_t));
	if(mqueue == NULL) {
		return NULL;
	}

	mqueue->owner = owner;
	mqueue->key = key;
	mqueue->id = id;

	return mqueue;
}

static void mq_free(mq_t * mqueue) {
	free(mqueue);
}


