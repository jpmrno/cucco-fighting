#include <server.h>
#include <slib.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> // TODO: Remove
#include <signal.h>
#include <library.h>

#define CONFIG_FILE_DEFAULT "config.ini"

static int handle(connection_t connection);
static void handle_int(int sign);
static int run(connection_t connection, int op);

static connection_t connection = NULL;

int main(int argc, char const * argv[]) {
	const char * config_file;
	int connection_numer = 0;

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
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, handle_int);
	signal(SIGCHLD, handle_int);

	while(TRUE) {
		connection_t connection_accepted;
		int ret, pid;

		connection_accepted = server_accept(connection);
		if(connection_accepted == NULL) {
			printf("Error en la nueva conexion!\n");
			exit(EXIT_FAILURE);
		}

		connection_numer++;

		pid = fork();
		if(pid == -1) {
			printf("Error en el fork!\n");
			exit(EXIT_FAILURE);
		}

		if(!pid) { // Child process
			server_ajar(connection);

			ret = handle(connection_accepted);

			server_close(connection_accepted);
			exit(ret);
		}

		printf("(%d) Desconectando nueva conexion...\n", connection_numer);
		server_ajar(connection_accepted);
	}

	printf("Desconectando principal conexion...\n");
	server_close(connection);

	return 0;
}

static int handle(connection_t connection) {
	int op;

	while(TRUE) {
		op = opcode(connection);
		run(connection, op);
	}

	return TRUE;
}

static void handle_int(int sign) {
	if(sign == SIGINT) {
		server_close(connection);
		exit(EXIT_FAILURE);
	} else if(sign == SIGCHLD) {
		int status;

		waitpid(-1, &status, 0);
	}
}

static int run(connection_t connection, int op) {
	switch(op) {
		case USER: {
			if(!login(connection)) {
				printf("Error en el login...\n");
				return FALSE;
			}
		} break;

		case MONEY: {
			if(!money(connection)) {
				printf("Error en el money...\n");
				return FALSE;
			}
		} break;

		case CUCCO_ADD: {
			if(!cucco_add(connection)) {
				printf("Error en el add...\n");
				return FALSE;
			}
		} break;

		case CUCCO_REMOVE: {
			if(!cucco_remove(connection)) {
				printf("Error en el remove...\n");
				return FALSE;
			}
		} break;

		case LIST: {
			if(list(connection)) {
				printf("Error en el list...\n");
				return FALSE;
			}
		} break;

		case BET: {
			if(!bet(connection)) {
				printf("Error en el bet...\n");
				return FALSE;
			}
		} break;

		case RESET: {
			if(!reset(connection)) {
				printf("Error en el reset...\n");
				return FALSE;
			}
		} break;

		case EXIT: {
			if(!logout(connection)) {
				printf("Error en el exit...\n");
				return FALSE;
			}
			exit(0);
		} break;
	}

	return TRUE;
}
