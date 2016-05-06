#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>

int main(int argc, char const * argv[]) {
	connection_t connection;

	connection = c_connect("127.0.0.1", 8080);
	if(connection == NULL) {
		printf("Error en la conexion!\n");
	}

	c_disconnect(connection);

	return 0;
}
