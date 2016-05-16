#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE_MAX 128

typedef struct {
	long mtype;       /* message type, must be > 0 */
	char mtext[BUFFER_SIZE_MAX];    /* message data */
} msgbuf;

int create_msq();
int send_message(int mtype, char* msg);
int recieve_msg();

// key_t key; /* key to be passed to msgget() */
// 	/*Process must share a common key in order to gain access to the queue*/
//
// int msgflg /* msgflg to be passed to msgget() */
// 	/* msgflg argument must be an octal integer with settings for the queue's permissions and control flags.*/
//

int mq_create(key_t key) {
	int msqid;

	msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
	if(msqid == -1) {
   		return -1;
   	}

	return msqid;
}

int mq_open(key_t key) {
	int msqid;

	msqid = msgget(key, 0666);
	if(msqid == -1) {
   		return -1;
   	}

	return msqid;
}


int mq_send(int msqid, int mtype, char * msg) {
	msgbuf buffer;

	buffer.mtype = mtype;
	strcpy(buffer.mtext, msg);

	return msgsnd(msqid, &buffer, BUFFER_SIZE_MAX, msgflg); // TODO: Flags?
}

msgbuf mq_receive(int msqid, char * msg){
	msgbuf buffer;

	return msgrcv(msqid, buffer, BUFFER_SIZE_MAX, 1, msgflg);
}






















#ifndef _MQUEUE_H_
#define _MQUEUE_H_

struct mq_t;
typedef struct mq_t * mqueue_t;

mqueue_t mq_make(const char * name);
void mq_remove(mqueue_t mqueue);
mqueue_t mq_connect(const char * name);
void mq_disconnect(mqueue_t mqueue);

#endif









#include <mqueue.h>
#include <define.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MESSAGES_MAX
#define MESSAGE_SIZE_MAX

#define PERMISSIONS 0666
#define FLAGS_MAKE O_RDONLY | O_CREAT | O_EXCL
#define FLAGS_OPEN O_WRONLY

typedef struct {
	char * name;
	mqd_t queue;
} mq_t;

static mqueue_t * mq_new(char * name, mqd_t queue);
static void mq_free(mqueue_t * mqueue);

mqueue_t mq_make(const char * name) {
	mq_t * mqueue;
	mqd_t queue;
	struct mq_attr attrs;

	attrs.mq_flags = 0;						// message queue flags. Flags: 0 or O_NONBLOCK
	attrs.mq_maxmsg = MESSAGES_MAX;			// maximum number of messages
	attrs.mq_msgsize = MESSAGE_SIZE_MAX;	// maximum message size
	attrs.mq_curmsgs = 0;					// number of messages currently queued

	queue = mq_open(name, FLAGS_MAKE, PERMITIONS, &attrs);
	if(queue == -1) {
		mq_unlink(name);
		return NULL;
	}

	mqueue = mq_new(name, queue);
	if(mqueue == NULL) {
		mq_close(queue);
		mq_unlink(name);
		return NULL;
	}

	return (mqueue_t) mqueue;
}

void mq_remove(mqueue_t mqueue) {
	mq_t * mq = (mq_t *) mqueue;

	if(mq != NULL) {
		if(mq->owner) {
			mq_unlink(mq->name);
		}
		mq_disconnect(mq);
	}
}

mqueue_t * mq_connect(const char * name) {
	mq_t * mqueue;
	mqd_t queue;

	queue = mq_open(name, FLAGS_OPEN);
	if(queue == -1) {
		return NULL;
	}

	mqueue = mq_new(name, queue);
	if(mqueue == NULL) {
		mq_close(queue);
		return NULL;
	}

	return (mqueue_t) mqueue;
}

void mq_disconnect(mqueue_t mqueue) {
	mq_t * mq = (mq_t *) mqueue;

	if(mq != NULL) {
		mq_close(mq->queue);
		mq_free(mq);
	}
}

static mq_t * mq_new(char * name, mqd_t queue) {
	mq_t * mqueue;

	mqueue = malloc(sizeof(mqueue_t));
	if(mqueue == NULL) {
		return NULL;
	}

	mqueue->name = malloc(sizeof(char) * (strlen(name) + 1));
	if(mqueue->name == NULL) {
		free(mqueue);
		return NULL;
	}

	strcpy(mqueue->name, name);
	mqueue->queue = queue;

	return mqueue;
}

static void mq_free(mq_t * mqueue) {
	if(mqueue != NULL) {
		free(mqueue->name);
		free(mqueue);
	}
}

