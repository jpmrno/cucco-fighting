#include <logger.h>
#include <define.h>
#include <stdio.h>
#include <mqueue.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>

#define USAGE_STRING "Usage: 'logger.app <stdout/file> [file_path]'.\n"

static void handle_int(int sign);

static int handle_stdout();
static int handle_file(const char * path);

static char * level(level_t level);

static mqueue_t mqueue;
static FILE * fp = NULL;

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
	} else {
		printf("FILE: %s\n", file_path);
	}

	mqueue = mq_make(1234);
	if(mqueue == NULL) {
		fprintf(stderr, "No se pudo crear la message queue.");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);

	if(file_path == NULL){
		handle_stdout();
	}else{
		if(handle_file(file_path)) {
			printf("No pude abrir el archivo guey!\n");
		}
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

static int handle_stdout() {
	system("clear");
	qmessage_t* msg = malloc(sizeof(qmessage_t));
	if(msg == NULL){
		exit(EXIT_FAILURE);
	}
	while(TRUE){
		mq_receive(mqueue, 0, msg);
		printf("%s %s\n", level(msg->level), msg->text);
	}
}

static int handle_file(const char * path) {

	qmessage_t* msg = malloc(sizeof(qmessage_t));
	if(msg == NULL){
		exit(EXIT_FAILURE);
	}
	fp = fopen(path, "wb");
	if(fp == NULL){
		return FALSE;
	}
	while(TRUE){
		mq_receive(mqueue, 0, msg);
		fprintf(fp, "%s %s\n", level(msg->level), msg->text);
	}
	fclose(fp);
	return TRUE;
}

static char * level(level_t level) {
	switch(level) {
		case LEVEL_INFO:
			return "Info:";
			break;
		case LEVEL_WARNING:
			return "Warning";
			break;
		case LEVEL_ERROR:
			return "Error: ";
			break;
	}
	return "";
}
