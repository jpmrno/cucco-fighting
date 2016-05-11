#include <server.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> // TODO: Remove
#include <signal.h>

#define CONFIG_FILE_DEFAULT "config.ini"

static int handle(connection_t connection);
static void handle_int(int sign);

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
	int random;

	srand(time(NULL)+(time_t)&random);
	random = rand()%10;
	printf("(%d) Me pude conectar!\n", random);
	sleep(random);

	static char * string = "HOLA!";
	server_send(connection, string, (size_t) 6);

	return 0;
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
