#include <string.h>
#include <define.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

static mqueue_t mq = NULL;

int log_open(){
	mq = mq_open(1234); // TODO: Define 1234

	if(mq == NULL) {
		return FALSE;
	}

	return TRUE;
}

int log_send(level_t level, char * message){
	qmessage_t * msg = malloc(sizeof(qmessage_t));

	if(mq == NULL || msg == NULL) {
		return FALSE;
	}

	msg->type = 1; // TODO: Define 1
	msg->level = level;
	memcpy(msg->text, message, sizeof(char) * (strlen(message) + 1));

	return mq_send(mq, msg);
}

void log_close() {
	if(mq != NULL) {
		mq_close(mq);
	}
}
