#include <dblib.h>
#include <define.h>
#include <sqlite3.h>
#include <stdio.h> // TODO: Remove

typedef sqlite3 dbsql_t;

#define DATABASE_PATH_DEFAULT "database.sqlite"

#define SQL_DATABASE_INIT "CREATE TABLE IF NOT EXISTS Clients (" \
"	ID integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE DEFAULT(0)," \
"	Name varchar NOT NULL UNIQUE," \
"	Money integer NOT NULL DEFAULT(50)" \
");" \
"CREATE TABLE IF NOT EXISTS Cuccos (" \
"	ID integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE DEFAULT(0)," \
"	Name varchar NOT NULL UNIQUE" \
");"

typedef int (*callback_t)(void *, int, char **, char **);

static int database_sql(dbsql_t * database, char * sql, callback_t callback);
static int callback(void * notUsed, int argc, char **argv, char ** azColName);

database_t database_open(const char * path) {
	dbsql_t * database;

	if(sqlite3_open(path, &database) != SQLITE_OK) {
		// TODO: Errores
		// fprintf(stderr, "<db error> can't open database: %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		return NULL;
	}

	if(!database_sql(database, SQL_DATABASE_INIT, NULL)) {
		sqlite3_close(database);
		return NULL;
	}

	return (database_t) database;
}

void database_close(database_t database) {
	dbsql_t * db = (dbsql_t *) database;

	if(db != NULL) {
		sqlite3_close(db);
	}
}

int database_cuccos_add(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	// http://stackoverflow.com/questions/2942370/how-to-insert-any-string-in-sqlite3-in-c
	char * sql = sqlite3_mprintf("INSERT OR IGNORE INTO Cuccos(Name) VALUES('%q')", name);
	int ret = database_sql(db, sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_get(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("SELECT ID, Name FROM Cuccos WHERE Name = '%q'", name);
	int ret = database_sql(db, sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_getn(database_t database, int id) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("SELECT ID, Name FROM Cuccos WHERE ID = %d", id);
	int ret = database_sql(db, sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_remove(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("DELETE FROM Cuccos WHERE Name = '%q'", name);
	int ret = database_sql(db, sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_removen(database_t database, int id) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("DELETE FROM Cuccos WHERE ID = %d", id);
	int ret = database_sql(db, sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_cuccos_list(database_t database) {
	dbsql_t * db = (dbsql_t *) database;
	int ret = database_sql(db, "SELECT * FROM Cuccos", callback);

	return ret;
}

int database_clients_add(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	// http://stackoverflow.com/questions/2942370/how-to-insert-any-string-in-sqlite3-in-c
	char * sql = sqlite3_mprintf("INSERT OR IGNORE INTO Clients(Name) VALUES('%q')", name);
	int ret = database_sql(db, sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_clients_get(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("SELECT ID, Name, Money FROM Clients WHERE Name = '%q'", name);
	int ret = database_sql(db, sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_clients_getn(database_t database, int id) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("SELECT ID, Name, Money FROM Clients WHERE ID = %d", id);
	int ret = database_sql(db, sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_clients_remove(database_t database, char * name) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("DELETE FROM Clients WHERE Name = '%q'", name);
	int ret = database_sql(db, sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_clients_removen(database_t database, int id) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("DELETE FROM Clients WHERE ID = %d", id);
	int ret = database_sql(db, sql, NULL);

	sqlite3_free(sql);

	return ret;
}

int database_clients_list(database_t database) {
	dbsql_t * db = (dbsql_t *) database;
	int ret = database_sql(db, "SELECT * FROM Clients", callback);

	return ret;
}

int database_clients_money(database_t database, char * name, int money) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("UPDATE Clients SET Money = Money + %d WHERE NAME = '%q'", money, name);
	int ret = database_sql(db, sql, callback);

	sqlite3_free(sql);

	return ret;
}

int database_clients_moneyn(database_t database, int id, int money) {
	dbsql_t * db = (dbsql_t *) database;
	char * sql = sqlite3_mprintf("UPDATE Clients SET Money = Money + %d WHERE ID = %d", money, id);
	int ret = database_sql(db, sql, callback);

	sqlite3_free(sql);

	return ret;
}

static int database_sql(dbsql_t * database, char * sql, callback_t callback) {
	char * error;

	if(sqlite3_exec(database, sql, callback, NULL, &error) != SQLITE_OK) {
		// TODO: Errors
		// fprintf(stderr, "<db error> SQL: %s\n", error);
		sqlite3_free(error);
		return FALSE;
	}

	return TRUE;
}

// TODO: Cambiar a q lo guarde en una estructura
static int callback(void * notUsed, int argc, char **argv, char ** azColName) {
	int i;

	for(i = 0; i < argc - 1; i++) {
	    printf("%s = %s\t", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

	return 0;
}
