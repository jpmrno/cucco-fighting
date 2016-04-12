c: leer alarm()

## Cliente
Persona q no tiene cuccos, apuesta por el listado de cuccos

### Comandos:

- **list:** lista de cuccos
- **bet cucco money:** apuesta a cucco, con tanta plata
- **cashup:** cierra la coneccion con el servidor y termina el proceso
- **money:** muestra la cant. de dinero disponible
- **caching:** reinicia la cantidad de dinero

### Todos/Consultas:

- Pregunta nombre al correr, ve si existe en base de datos y gettea datos? O si no existe, escribe datos

## Servidor

### Comandos:

- **list:** lista de cuccos
- **mkcucco name:** añade un cucco
- **rmcucco name:** remueve un cucco
- **cashup name:** cierra la coneccion con el cliente (name)
- **figth:** comienza la pelea

### Todos/Consultas:

- Cuando un cliente apueste (el primero en apostar) inicia el timer para que el resto de los clientes apueste
