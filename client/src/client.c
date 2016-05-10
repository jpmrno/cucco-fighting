#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
			fprintf(stderr, "Usage: 'server.app [config_file]'.\n");
			exit(EXIT_FAILURE);
		}
	}

	connection = server_connect(config_file);
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	char * string = malloc(sizeof(char) * 10);
	if(string == NULL) {
		return 1;
	}
	server_receive(connection, string, (size_t) 10);


	while(1){
		printf("%s\n", string);
		
		
	}

	



	server_disconnect(connection);

	return 0;
}
