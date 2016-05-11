#include <logger.h>
#include <define.h>
#include <mqueue.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>

#define USAGE_STRING "Usage: 'logger.app <stdout/file> [file_path]'.\n"

static void handle_int(int sign);

static mqueue_t mqueue;

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

	while(TRUE) {

	}

	mq_remove(mqueue);

	return 0;
}

// int logger_stdio() {

// }

// int logger_file(char * path) {
// 	FILE * file;

// 	file = fopen(path, "a+");
// 	if(file == NULL) {
// 		return FALSE;
// 	}
// }

static void handle_int(int sign) {
	if(sign == SIGINT) {
		mq_remove(mqueue);
		exit(EXIT_FAILURE);
	}
}
