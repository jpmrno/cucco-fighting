c: leer alarm()

# Cliente
Persona q no tiene cuccos, apuesta por el listado de cuccos

### Comandos:

- **list:** lista de cuccos
- **bet cucco money:** apuesta a cucco, con tanta plata
- **bet -n # money:** apuesta a cucco numero #, con tanta plata
- **cashup:** cierra la coneccion con el servidor y termina el proceso
- **money/wallet:** muestra la cant. de dinero disponible
- **caching:** reinicia la cantidad de dinero

### Todos/Consultas:

- Pregunta nombre al correr, ve si existe en base de datos y gettea datos? O si no existe, escribe datos
- './client name'?

# Servidor

### Comandos:

- **lscuccos:** lista de cuccos
- **mkcucco name:** añade un cucco
- **rmcucco name:** remueve un cucco
- **lsclients:** lista de clientes con todos sus datos
- **rmclient name:** cierra la coneccion con el cliente (name)
- **figth:** comienza la pelea

### Todos/Consultas:

- Cuando un cliente apueste (el primero en apostar) inicia el timer para que el resto de los clientes apueste
