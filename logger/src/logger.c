#include <logger.h>
#include <define.h>
#include <mqueue.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>

#define USAGE_STRING "Usage: 'logger.app <stdout/file> [file_path]'.\n"

static void handle_int(int sign);
static int handle_file(char * file_path);
static int write_file(FILE* fp, int priority, char* msg);

static mqueue_t mqueue;
static FILE* fp = NULL;

int main(int argc, char const * argv[]) {
	const char * file_path;

	switch(argc) {
		case 2: {
			if(strcmp(argv[1], "stdout")) {
				fprintf(stderr, USAGE_STRING);
				exit(EXIT_FAILURE);
			}
			file_path = NULL;
		} break;

		case 3: {
			if(strcmp(argv[1], "file")) {
				fprintf(stderr, USAGE_STRING);
				exit(EXIT_FAILURE);
			}
			file_path = argv[2];
		} break;

		default: {
			fprintf(stderr, USAGE_STRING);
			exit(EXIT_FAILURE);
		}
	}

	if(file_path == NULL) {
		printf("STDOUT\n");
		// ret = logger_stdio();
	} else {
		printf("FILE: %s\n", file_path);
		// ret = logger_file(file_path);
	}

	mqueue = mq_make(1234);
	if(mqueue == NULL) {
		fprintf(stderr, "No se pudo crear la message queue.");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);

	message_t* msg = malloc(sizeof(message_t));
	if(msg == NULL){
		exit(EXIT_FAILURE);
	}
	
	if(file_path == NULL){
		while(TRUE){
			mq_receive(mqueue, 0 ,msg);
			printf("Type %d: %s\n", msg->type, msg->message);
		}
	}else{
		handle_file(file_path);
	}	

	mq_remove(mqueue);

	return 0;
}


static void handle_int(int sign) {
	if(sign == SIGINT) {
		fclose(fp);
		mq_remove(mqueue);
		exit(EXIT_FAILURE);
	}
}

static int handle_file(char * path) {
	fp = fopen(path, "w+");
	if(fp == NULL){
		return FALSE;
	}

	while(TRUE){
		mq_receive(mqueue, 0, msg);
		write_file(fp, msg->type, msg->message);
	}

	fclose(fp);
}

static int write_file(FILE* fp, int priority, char* msg){
	int ret;
	ret = fwrite(priority, sizeof(int), 1, fp);
	if(ret != 0){
		return -1;
	}
	ret = fwrite(msg, strlen(msg), 1, fp);
	if(ret != 0){
		return -1;
	}
}
