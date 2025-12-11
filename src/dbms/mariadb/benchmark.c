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

void create_indexes(void *driver){
	MYSQL *mysql = (MYSQL*) driver;
	if (mysql_query(mysql, "CREATE INDEX idx_table3 ON test_table3 BTREE ON test_table3")) {
		close_connection(mysql);
	}
	if (mysql_query(mysql, "CREATE INDEX idx_table4 ON test_table4 BTREE ON test_table4")) {
		close_connection(mysql);
	}
}

void setUpDatabase(void *driver){
    MYSQL *mysql = (MYSQL*) driver;
    if (mysql_query(mysql, "DROP TABLE IF EXISTS test_table")){
       close_connection(mysql);
    }
    if (mysql_query(mysql, "DROP TABLE IF EXISTS test_table2")){
       close_connection(mysql);
    }
    if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS test_table(id INTEGER AUTO_INCREMENT PRIMARY KEY, field1 INTEGER, field2 DOUBLE, field3 INTEGER, field4 DOUBLE, field5 INTEGER, field6 DOUBLE)ENGINE=InnoDB")){
       close_connection(mysql);
    }
    if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS test_table2(id INTEGER AUTO_INCREMENT PRIMARY KEY, field1 INTEGER, field2 DOUBLE, field3 INTEGER, field4 DOUBLE, field5 INTEGER, field6 DOUBLE)ENGINE=InnoDB")){
        close_connection(mysql);
    }

    if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS test_table3(id INTEGER AUTO_INCREMENT PRIMARY KEY, field1 INTEGER, field2 DOUBLE, field3 INTEGER, field4 DOUBLE, field5 INTEGER, field6 DOUBLE)ENGINE=InnoDB")){
       close_connection(mysql);
    }
    if (mysql_query(mysql, "CREATE TABLE IF NOT EXISTS test_table4(id INTEGER AUTO_INCREMENT PRIMARY KEY, field1 INTEGER, field2 DOUBLE, field3 INTEGER, field4 DOUBLE, field5 INTEGER, field6 DOUBLE)ENGINE=InnoDB")){
        close_connection(mysql);
    }

    // insert mock data into test_table2
    for (int i = 0; i < N_STATS; i++){
        insert_mock_data_table2(mysql);
		insert_mock_data_table3(mysql);
		insert_mock_data_table4(mysql);
    }
	create_indexes(mysql);
}

void insert_mock_data_table2(void *driver){
    MYSQL_BIND bind[6];
    for (int i = 0; i < 6; i++){
        bind[i].buffer_type = MYSQL_TYPE_LONG;
        int number = get_random_int();
        bind[i].buffer = &number;
        bind[i].buffer_length = 4;
    }
    return executer(driver, bind,  "INSERT INTO test_table2(field1, field2, field3, field4, field5, field6) VALUES (?,?,?,?,?,?)");
}

void insert_mock_data_table3(void *driver){
	MYSQL_BIND bind[6];
	for (int i = 0; i < 6; i++){
		bind[i].buffer_type = MYSQL_TYPE_LONG;
		int number = get_random_int();
		bind[i].buffer = &number;
		bind[i].buffer_length = 4;
	}
	executer(driver, bind, "INSERT INTO test_table3(field1, field2, field3, field4, field5, field6) VALUES (?,?,?,?,?,?)");
}

void insert_mock_data_table4(void *driver){
	MYSQL_BIND bind[6];
	for (int i = 0; i < 6; i++){
		bind[i].buffer_type = MYSQL_TYPE_LONG;
		int number = get_random_int();
		bind[i].buffer = &number;
		bind[i].buffer_length = 4;
	}
	executer(driver, bind, "INSERT INTO test_table4(field1, field2, field3, field4, field5, field6) VALUES (?,?,?,?,?,?)");
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

int select_from_where_inner_join(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table.field1, test_table.field2, test_table.field3 FROM test_table INNER JOIN test_table2 ON test_table.id=test_table2.id WHERE id = ?");
}

int select_from_where_inner_join_with_index(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table3.field3, test_table4.field4 FROM test_table3 JOIN test_table4 ON test_table.id=test_table4.id WHERE id > ?");
}

int select_from_where_inner_join_with_index_hash(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table5.field4, test_table6.field5 FROM test_table5 INNER JOIN test_table6 ON test_table5.id=test_table6.id WHERE id = ?");
}

int select_from_where_outer_join(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table1.field2, test_table2.field5 FROM test_table1 OUTER JOIN test_table2 ON test_table1.id=test_table2.id WHERE id = ?");
}

int select_from_where_outer_join_with_index(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table3.field4, test_table4.field6 FROM test_table3 OUTER JOIN test_table4 ON test_table3.id=test_table4.id WHERE id < ?");
}

int select_from_where_outer_join_with_index_hash(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table5.field5, test_table6.field4 FROM test_table5 OUTER JOIN test_table6 ON test_table5.id=test_table6.id WHERE id = ?");
}

int select_from_where_right_join(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table.field1, test_table2.field2 FROM test_table RIGHT JOIN test_table2 ON test_table.id=test_table2.id WHERE id = ?");
}

int select_from_where_right_join_with_index(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
   return executer(driver, bind, "SELECT test_table3.field3, test_table4.field4 FROM test_table3 RIGHT JOIN test_table4 ON test_table3.id=test_table4.id WHERE id < ?");
}

int select_from_where_right_join_with_index_hash(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table5.field5, test_table6.field6 FROM test_table5 RIGHT JOIN test_table6 ON test_table5.id=test_table6.id WHERE id = ?");
}

int select_from_where_left_join(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT test_table.field4, test_table2.field6 FROM test_table LEFT JOIN test_table2 ON test_table.id=test_table2.id WHERE id = ?");
}

int select_from_where_left_join_with_index(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT * from test_table3 LEFT JOIN test_table4 on test_table3.id=test_table4.id WHERE id < ?");
}

int select_from_where_left_join_with_index_hash(void *driver, int id){
    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    int number = get_random_database_index();
    bind[0].buffer = &number;
    bind[0].buffer_length = 4;
    return executer(driver, bind, "SELECT * FROM test_table5 LEFT JOIN test_table6 on test_table5.id=test_table6.id WHERE id = ?");
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
        "/run/mysqld/mysqld.sock",                 // Path to socket file
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

void perform_select_with_index(double stats[]){
    perform_operation(stats, &select_with_index);
}

void perform_select_from_where_inner_join(double stats[]){
    perform_operation(stats, &select_from_where_inner_join);
}
void select_from_where_inner_join_with_index(double stats[]){
    perform_operation(stats, &select_from_where_inner_join_with_index);
}

void select_from_where_inner_join_with_index_hash(double stats[]){
    perform_operation(stats, &select_from_where_inner_join_with_index_hash);
}

void select_from_where_outer_join(double stats[]){
    perform_operation(stats, &select_from_where_outer_join);
}

void select_from_where_outer_join_with_index(double stats[]){
    perform_operation(stats, &select_from_where_outer_join_with_index);
}

void select_from_where_outer_join_with_index_hash(double stats[]){
    perform_operation(stats, &select_from_where_outer_join_with_index_hash);
}
void select_from_where_right_join(double stats[]){
    perform_operation(stats, &select_from_where_right_join);
}
void select_from_where_right_join_with_index(double stats[]){
    perform_operation(stats, &select_from_where_right_join_with_index);
}
void select_from_where_right_join_with_index_hash(double stats[]){
    perform_operation(stats, &select_from_where_right_join_with_index_hash);
}
void select_from_where_left_join(double stats[]){
    perform_operation(stats, &select_from_where_left_join);
}
void select_from_where_left_join_with_index(double stats[]){
    perform_operation(stats, &select_from_where_left_join_with_index);
}
void select_from_where_left_join_with_index_hash(double stats[]){
    perform_operation(stats, &select_from_where_left_join_with_index_hash);
}

void perform_insert(double stats[]){
    perform_operation(stats, &insert);
}
void perform_update(double stats[]){
    perform_operation(stats, &update);
}
void perform_delete(double stats[]){
    perform_operation(stats, &delete_from);
}