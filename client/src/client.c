#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>

#define CONFIG_FILE_DEFAULT "config.ini"

int main(int argc, char const * argv[]) {
	connection_t connection;
	const char * config_file;

	switch(argc) {
		case 1: {
			config_file = CONFIG_FILE_DEFAULT;
		} break;

		case 2: {
			config_file = argv[1];
		} break;

		default: {
			printf("Usage: 'server.app [config_file]'.\n"); // TODO: perror()
			return 1;
		}
	}

	connection = server_connect(config_file);
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	server_disconnect(connection);

	return 0;
}
