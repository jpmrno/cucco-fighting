#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const * argv[]) {
	connection_t connection;

	if(argc != 2) {
		printf("Falta archivo de configuracion.\n"); // TODO: perror()
		return 1;
	}

	connection = server_connect(argv[1]);
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	server_disconnect(connection);

	return 0;
}
