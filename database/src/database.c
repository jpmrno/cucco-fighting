#include <database.h>
#include <dblib.h>
#include <stdio.h>
#include <stdlib.h>
#include <smemory.h>
#include <signal.h>

#define DATABASE_PATH_DEFAULT "database.sql"

static void handle_int(int sign);
static int handle(void * buffer, int size);

static database_t database = NULL;
static smemory_t server = NULL;

int main(int argc, char const * argv[]) {
	const char * database_path;
	void * buffer;
	size_t size;
	int ret = 0;

	switch(argc) {
		case 1: {
			database_path = DATABASE_PATH_DEFAULT;
		} break;

		case 2: {
			database_path = argv[1];
		} break;

		default: {
			fprintf(stderr, "Usage: 'database.app [database_path]'.\n");
			exit(EXIT_FAILURE);
		}
	}

	database = database_open(database_path);
	if(database == NULL) {
		fprintf(stderr, "Can't open database.\n");
		exit(EXIT_FAILURE);
	}

	server = smemory_make(8080); // TODO: define 8080
	if(server == NULL) {
		fprintf(stderr, "Can't create the main connection.\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);

	while(!ret) {
		smemory_read(server, &buffer, &size);

		ret = handle(buffer, size);
	}

	smemory_remove(server);
	database_close(database);

	return ret == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		smemory_remove(server);
		database_close(database);
	}
}

static int handle(void * buffer, int size) { // TODO: Change
	if(!smemory_write(server, "Recibido", 8)) {
		return -1;
	}

	return 0;
}
