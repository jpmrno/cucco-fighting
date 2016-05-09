#include <database.h>
#include <dblib.h>
#include <stdio.h>
#include <stdlib.h>

#define DATABASE_PATH_DEFAULT "database.sql"

static database_t database = NULL;

int main(int argc, char const * argv[]) {
	const char * database_path;

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
		fprintf(stderr, "No se pudo crear/abrir la base de datos.\n");
		exit(EXIT_FAILURE);
	}

	database_cuccos_list(database);

	// TODO: Comunicacion con el servidor

	database_close(database);

	return 0;
}
