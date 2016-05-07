#include <server.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// #include <signal.h>

// void stop(int val);
void handle(connection_t connection);

static volatile int running = TRUE;

int main(int argc, char const * argv[]) {
	int connection_numer = 0;
	connection_t connection_accepted;

	connection_t connection = server_start();
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	while(running) {
		connection_accepted = server_incoming(connection);
		if(connection_accepted == NULL) {
			printf("Error en la nueva conexion!\n");
			return 1;
		}

		connection_numer++;

		if(!fork()) { // Child process
			//server_disconnect(connection);
			handle(connection_accepted);

			server_stop(connection_accepted);
			exit(0); // TODO: Porq no return 0?
		}

		printf("(%d) Desconectando nueva conexion...\n", connection_numer);
		server_disconnect(connection_accepted);
	}

	printf("Desconectando principal conexion...\n");
	server_stop(connection);

	return 0;
}

// void stop(int val) {
//     running = FALSE;
// }

void handle(connection_t connection) {
	printf("Me pude conectar!\n");
	printf("Termine con el cliente!\n");
}

