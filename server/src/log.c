#include <string.h>
#include <define.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

static char * level_string(level_t level);
static mqueue_t mq = NULL;

#define KEY_ID 3

int log_open() {
	key_t key = ftok("database.sql", KEY_ID);

	mq = mq_open(key);

	if(mq == NULL) {
		return FALSE;
	}

	return TRUE;
}

int log_send(level_t level, char * message) {
	qmessage_t * msg = malloc(sizeof(qmessage_t));

	if(msg == NULL) {
		return FALSE;
	}

	if(mq == NULL) {
		printf("%s: %s\n", level_string(level), message);
		return TRUE;
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

static char * level_string(level_t level) {
	switch(level) {
		case LEVEL_INFO:
			return "Info";
			break;
		case LEVEL_WARNING:
			return "Warning";
			break;
		case LEVEL_ERROR:
			return "Error";
			break;
	}

	return "";
}
