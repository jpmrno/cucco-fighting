#include <server.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define CONFIG_FILE_DEFAULT "config.ini"

int handle(connection_t connection);

int main(int argc, char const * argv[]) {
	connection_t connection, connection_accepted;
	int connection_numer = 0, ret;
	const char * config_file;

	switch(argc) {
		case 1: {
			config_file = CONFIG_FILE_DEFAULT;
		} break;

		case 2: {
			config_file = argv[1];
		} break;

		default: {
			fprintf(stderr, "Usage: 'server.app [config_file]'.\n");
			exit(EXIT_FAILURE);
		}
	}

	connection = server_open(config_file);
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	while(TRUE) {
		connection_accepted = server_accept(connection);
		if(connection_accepted == NULL) {
			printf("Error en la nueva conexion!\n");
			return 1;
		}

		connection_numer++;

		if(!fork()) { // Child process
			server_ajar(connection);

			ret = handle(connection_accepted);

			server_close(connection_accepted);
			exit(ret); // TODO: Porq no return ret;?
		}

		printf("(%d) Desconectando nueva conexion...\n", connection_numer);
		server_ajar(connection_accepted);
	}

	printf("Desconectando principal conexion...\n");
	server_close(connection);

	return 0;
}

int handle(connection_t connection) {
	printf("Me pude conectar!\n");

	sleep(10);

	static char * string = "HOLA!";
	server_send(connection, string, (size_t) 6);

	return 0;
}

