#include <client.h>
#include <define.h>
#include <comapi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const * argv[]) {
	connection_t connection;

	connection = c_connect("127.0.0.1", 8080);
	if(connection == NULL) {
		printf("Error en la conexion!\n");
		return 1;
	}

	sleep(10);
	c_disconnect(connection);

	return 0;
}
