#include <client.h>
#include <define.h>
#include <clib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define CONFIG_FILE_DEFAULT "config.ini"

static void handle_int(int sign);

static connection_t connection = NULL;

int main(int argc, char const * argv[]) {
	const char * config_file;
	int ret;

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
		fprintf(stderr, "No se pudo conectar al servidor.\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);

	ret = login(connection, "JUAN BARTOLOME NAVAS");
	if(!ret) {
		fprintf(stderr, "No se pudo loguear al servidor.\n");
		server_disconnect(connection);
	}

	ret = money(connection);
	if(!ret) {
		fprintf(stderr, "No se pudo moneiar al servidor.\n");
		server_disconnect(connection);
	}

	ret = cucco_add(connection, "La mas rapida");
	if(!ret) {
		fprintf(stderr, "No se pudo addear al servidor.\n");
		server_disconnect(connection);
	}

	ret = cucco_remove(connection, "La mas rapida");
	if(!ret) {
		fprintf(stderr, "No se pudo remove al servidor.\n");
		server_disconnect(connection);
	}

	// ret = list(connection);
	// if(!ret) {
	// 	fprintf(stderr, "No se pudo listear al servidor.\n");
	// 	server_disconnect(connection);
	// }

	ret = bet(connection, "La mas rapida", 20.4);
	if(!ret) {
		fprintf(stderr, "No se pudo bettear al servidor.\n");
		server_disconnect(connection);
	}

	ret = reset(connection);
	if(!ret) {
		fprintf(stderr, "No se pudo resetear al servidor.\n");
		server_disconnect(connection);
	}

	ret = logout(connection);
	if(!ret) {
		fprintf(stderr, "No se pudo logoutear al servidor.\n");
		server_disconnect(connection);
	}

	server_disconnect(connection);

	return 0;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		server_disconnect(connection);
		exit(EXIT_FAILURE);
	}
}
