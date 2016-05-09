#include <server.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int handle(connection_t connection);

int main(int argc, char const * argv[]) {
	int connection_numer = 0, ret;
	connection_t connection_accepted;

	if(argc != 2) {
		// TODO: Default
		printf("Falta archivo de configuracion.\n"); // TODO: perror()
		return 1;
	}

	connection_t connection = server_open(argv[1]);
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

	return 0;
}

