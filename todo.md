c: leer alarm()

# Cliente
Persona q no tiene cuccos, apuesta por el listado de cuccos

### Comandos:

- **list:** lista de cuccos
- **bet cucco money:** apuesta a cucco, con tanta plata
- **bet -n # money:** apuesta a cucco numero #, con tanta plata
- **cashup:** cierra la conexion con el servidor y termina el proceso
- **money/wallet:** muestra la cant. de dinero disponible
- **kaching:** reinicia la cantidad de dinero

### Todos/Consultas:

- Pregunta nombre al correr, ve si existe en base de datos y gettea datos? O si no existe, escribe datos
- './client name'?

# Servidor

### Comandos:

- **cuccos:** lista de cuccos
- **cuccos add name:** añade un cucco
- **cuccos remove name:** remueve un cucco
- **clients:** lista de clientes con todos sus datos
- **clients remove name:** cierra la conexion con el cliente (name)
- **figth:** comienza la pelea

### Todos/Consultas:

- Cuando un cliente apueste (el primero en apostar) inicia el timer para que el resto de los clientes apueste
