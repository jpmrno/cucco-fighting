#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const * argv[]) {
	connection_t connection;

	connection = server_connect();
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	server_stop(connection);

	return 0;
}
