#define _POSIX_C_SOURCE 200809L
#include <operations.h>
#include <mysql.h>
#include <string.h>
#include <ctype.h>

void get_filename(char *name, int length){
    snprintf(name, length, "mariadb.csv");
}

void close_connection(void *driver){
    MYSQL *mysql = (MYSQL*) driver;
    fprintf(stderr, "Error(%d) [%s] %s\n", mysql_errno(mysql),
                                            mysql_sqlstate(mysql),
                                  mysql_error(mysql));
    mysql_close(mysql);
    exit(-1);
}

void close_stmt_connection(MYSQL_STMT *stmt){
    fprintf(stderr, "Error(%d), [%s], %s\n", mysql_stmt_errno(stmt),
                                           mysql_stmt_sqlstate(stmt),
                                           mysql_stmt_error(stmt));
}

void show_stmt_error(MYSQL_STMT *stmt){
    fprintf(stderr, "Error(%d), [%s] %s\n", mysql_stmt_errno(stmt),
                                        mysql_stmt_sqlstate(stmt),
                                        mysql_stmt_error(stmt));
    exit(-1);
}

void setUpDatabase(void *driver){
    MYSQL *mysql = (MYSQL*) driver;
     if (mysql_query(mysql, "DROP TABLE IF EXISTS test_table")){
        close_connection(mysql);
     }
     if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS test_table(id INTEGER AUTO_INCREMENT PRIMARY KEY, field1 INTEGER, field2 DOUBLE, field3 INTEGER, field4 DOUBLE, field5 INTEGER, field6 DOUBLE)ENGINE=InnoDB")){
        close_connection(mysql);
     }
}

int executer(void *driver, MYSQL_BIND *bind, char *query){
    MYSQL *conn = (MYSQL*) driver;
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (mysql_stmt_prepare(stmt, query, strnlen(query, MAX_SQL_LENGTH))){
       fprintf(stderr, "Query failed: %s\n", query);
       close_stmt_connection(stmt);
       mysql_close(conn);
    }
    mysql_stmt_bind_param(stmt, bind);
    // execute
    if (mysql_stmt_execute(stmt)) {
        close_stmt_connection(stmt);
        mysql_close(conn);
    }
    mysql_stmt_free_result(stmt);
    return 0;
}

int select(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT * FROM test_table WHERE id=(?) LIMIT 1"); // seleccionar un solo elemento.
}
int insert(void *driver, int id){
    MYSQL_BIND bind[6];
    for (int i = 0; i < 6; i++){
        bind[i].buffer_type = MYSQL_TYPE_LONG;
        int number = get_random_int();
        bind[i].buffer = &number;
        bind[i].buffer_length = 4;
    }
    return executer(driver, bind,  "INSERT INTO test_table(field1, field2, field3, field4, field5, field6) VALUES (?,?,?,?,?,?)");
}
int update(void *driver, int id){
    MYSQL_BIND bind[2];
    int number = get_random_int();
    int idx = get_random_database_index();
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &idx;
    bind[1].buffer_length = 4;
    return executer(driver, bind, "UPDATE test_table SET field1=(?) WHERE id=(?)"); // actualizar un solo campo de la base de datos
}

void perform_operation(double stats[], database_single_operation op){
    MYSQL *conn;
    if (!(conn = mysql_init(0))){
      fprintf(stderr, "unable to initialize connection struct\n");
      exit(1);
   }
   if (!mysql_real_connect(
         conn,                 // Connection
         "localhost",// Host
         "test_user",            // User account
         "chaochao33356",   // User password
         "test_databases",               // Default database
         3306,                 // Port number
        "/tmp/mysql.sock",                 // Path to socket file
         0                     // Additional options
      ))
   {
      // Report the failed-connection error & close the handle
      fprintf(stderr, "Error connecting to Server: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }
   setUpDatabase(conn);
   //perform operation
   clock_t start, end;
   for (int i = 0; i < N_STATS; i++){
    start = clock();
        (*op)(conn, i);
    end = clock();
     stats[i] = (double) (end - start) / CLOCKS_PER_SEC;
   }
   mysql_close(conn);
}

void perform_select(double stats[]){
    perform_operation(stats, &select);
}
void perform_insert(double stats[]){
    perform_operation(stats, &insert);
}
void perform_update(double stats[]){
    perform_operation(stats, &update);
}