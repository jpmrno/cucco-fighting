Cucco Fighting!
ITBA - Sistemas Operativos - TPE
16 de Mayo de 2016


Integrantes:
  - Francisco Bartolome: 54308
  - Natalia Navas: 53559
  - Juan Moreno: 54182


El trabajo consta de un sistema de apuestas ilegales de peleas de cuccos, en el que los usuarios podrán apostar a determinado cucco en una pelea y ganar plata de ser victorioso.

El proyecto consta de cuatro programas: uno para el servidor, uno para cada cliente, uno para el logger y otro que refiere a la base de datos. El cliente y el servidor se pueden comunicar tanto por sockets como por named pipes, siendo en el tiempo de compilación el momento donde se escogerá cuál de estas implementaciones se utilizará.

Tanto el servidor como el cliente leen de un archivo de configuracion los datos que necesita cada implementacion y si no lo encuentra utiliza datos por default.

Además, el servidor se comunicará con el logger mediante el uso message queues.  A su vez se comunicara con la base de datos utilizando shared memory y semáforos correspondientes, cuando se le haga un pedido a la base de datos esta escribirá en la memoria compartida y al momento de leer el servidor lo hace leyendo este espacio de memoria. Se eligió implementar memoria compartida en vez de pipes, ya que nos pareció un método más eficiente para compartir información entre múltiples clientes y un servidor.

Tanto el programa del cliente como el del servidor cuentan con tres capas de abstracción: el app (client.c y server.c) con la que el usuario interactúa directamente, el middle (clib.c y slib.c) donde se prepara para mandar y recibir los datos y que hace con ellos, y la capa de transporte. Esta última es donde se envían y se reciben los datos, y es indiferente tanto para el cliente como para el servidor.

El servidor realiza un fork() cada vez que un nuevo cliente se conecta con el, de esta manera hay un proceso del servidor que se encarga de ver si hay nuevas conexiones, mientras que hay un proceso del servidor que atiende a cada cliente conectado. Además, los procesos del servidor que atienden a cada cliente interactúan entre sí a la hora de que haya una pelea, de esta manera todos los clientes participan en la misma pelea. La pelea se realiza una vez que todos los usuarios conectados hayan apostado. Para realizar esto se usaron variables globales (mediante el uso de mmap() y el adecuado uso de semáforos) entre los distintos procesos del servidor.


Cómo correr el programa:

	Para correr el programa se necesita tener gcc-5.

Posicionarse sobre /cucco-fighting
En la terminal ejecutar: make all impl=XXX, donde en XXX se debe escribir pipes o sockets para elegir la implementación de transporte (pipes/sockets).

Ejecutar los programas en el siguiente orden:
El dataBase ejecutando './database/database.app'
El logger ejecutando './logger/logger.app <stdout/file> [file_path]' (en caso de no correrlo, el servidor escribe en stdout)
El servidor ejecutando './server/server.app'
El cliente ejecutando './client/client.app'


Las aplicaciones:

  - Cliente

Se pueden ejecutar los siguiente comandos:

		- list: devuelve una lista de los cuccos que están listos para la acción!
		- add: añade un cucco nuevo.
		- remove: remueve un cucco existente, siempre y cuando exista dicho cucco.
		- bet: permite apostar determinada cantidad de plata a un cucco y realiza la pelea. Si el cucco al que 		- apostó el usuario resulta ganador, el usuario ganará la cantidad de plata apostada. En caso contrario 		- perderá dicha plata. Si se apuesta más plata de la que se tiene, la plata apostada será la total.
		- wallet: devuelve la plata que dispone el usuario.
		- kaching: reinicia la cantidad de plata que dispone el usuario a 50 cuccope$o$.
		- help: muestra todos los comandos que se pueden realizar.
		- clear: limpia la pantalla.
		- exit: termina la ejecución del programa.

  - Server

Desde el lado del servidor están las funciones que son llamadas desde la capa de transporte del cliente.  Estas funciones escriben y leen en shared memory que es lo que se quiere buscar de la base de datos.  La base de datos escribe en este mismo lugar y luego el servidor lo lee, se comunica con el cliente y le envia esta informacion.

  - Logger

Desde el lado del logger se puede tanto escribir en un archivo como en el standard output.  Hay tres tipos de logs: INFO, WARNING y ERROR.  Desde el lado del servidor se agregan mensajes al message queue, y desde el lado del logger se recibe este mensaje y se lee.

En el caso de que el logger no este corriendo, los logs serán impresos en el servidor.


Bug:

  - Existe un bug que en determinadas circunstancias la librería utilizada para la serialización falla al leer los datos de retorno de la base de datos. Estas circunstancias son cuando un cliente se desconecta.


Librerias:

  - TPL (http://troydhanson.github.io/tpl/userguide.html) para serializacion. Escribe variables a un buffer.
  - sqlite3.
  - PCG Random Number Generation for C (http://www.pcg-random.org/download.html) para generar numeros random.
  - MinIni (http://www.compuphase.com/minini.htm) para archivos de configuracion INI.
  - Concurrent Bash (https://github.com/themattrix/bash-concurrent). Se utilizo para probar muchas conexiones simultaneas al servidor y comprobar que las implementaciones de la capa de transporte funcionan adecuadamente.
