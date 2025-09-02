#include <operations.h>
#include <sqlite3.h>

#define N_ARGS 7

void get_filename(char *name, int length){
    snprintf(name, length, "sqlite.csv");
}


void close_connection(void *driver, void *stmt, char *msg, int rc){
    sqlite3 *db = (sqlite3*) driver;
    sqlite3_stmt *st = (sqlite3_stmt*) stmt;
    fprintf(stderr, "SQLITE ERROR, MESSAGE %s, RETRUN CODE: %i\n", msg, rc);
    sqlite3_finalize(st);
    sqlite3_close(db);
    exit(-1);
}

void close_exec_connection(void *driver, char *msg, int rc){
    sqlite3 *db = (sqlite3* )driver;
    if (rc != SQLITE_OK){
        fprintf(stderr, "SQLITE ERROR, MESSAGE: %s, RETURN CODE: %i\n",msg, rc);
        sqlite3_free(msg);
        sqlite3_close(db);
        exit(-1);    
    }

}

void setUpDatabase(void *driver){
    sqlite3 *db = (sqlite3*) driver;
    char *errMsg;
    int rc = sqlite3_exec(db, "DROP TABLE IF EXISTS test_table",NULL, NULL, &errMsg);
    if (rc != SQLITE_OK){
        close_exec_connection(driver, errMsg, rc);
    }
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table(id INTEGER, field1 DOUBLE, field2 INTEGER, field3 DOUBLE, field4 INTEGER, field5 DOUBLE, field6 INTEGER)", NULL, NULL, &errMsg);
    if (rc != SQLITE_OK){
        close_exec_connection(driver, errMsg, rc);
    }
    sqlite3_free(errMsg);
}

int executer(void *driver, void *stmt){
    //sqlite3 *db = (sqlite3*) driver;
    sqlite3_stmt *prep_stmt = (sqlite3_stmt*) stmt;
    int rc = sqlite3_step(prep_stmt);
    if (rc != SQLITE_DONE){
        close_connection(driver, stmt, "error while evaluating statement", rc);
    }
    //sqlite3_free(errMsg);
    sqlite3_finalize(stmt);
    return 0;
}

void prepare(void *driver, char *query, void **stmt){
    sqlite3 *db = (sqlite3*) driver;
    sqlite3_stmt **st = (sqlite3_stmt**) stmt;
    int rc = sqlite3_prepare_v2(db, query, MAX_SQL_LENGTH, st, NULL);
    if (rc != SQLITE_OK){
        close_connection(db, *st, "error while  preparing statement", rc);
    }
}

int select(void *driver, int id){
    sqlite3_stmt *stmt;
    char query[MAX_SQL_LENGTH] = "SELECT * FROM test_table WHERE id=(?1) LIMIT 1";
    prepare(driver, query, (void **) &stmt);
    sqlite3_bind_int(stmt, 1, get_random_database_index());
    return executer(driver, stmt);
}

int insert(void *driver, int id){
    sqlite3_stmt *stmt;
    char query[MAX_SQL_LENGTH] = "INSERT INTO test_table(id, field1, field2, field3, field4, field5, field6) VALUES (?1,?2,?3,?4,?5,?6,?7)";
    prepare(driver, query,(void**) &stmt);
    sqlite3_bind_int(stmt, 1,id);
    int rc;
    for (int i = 2; i < N_ARGS; i++){
        if ((i & 1) == 0){ // it is even, so it will bind an integer
            int b = get_random_int();
            rc = sqlite3_bind_int(stmt, i, b);
            if (rc != SQLITE_OK){
                close_connection(driver, stmt, "error while binding param", rc);
            }
        }else{
            rc = sqlite3_bind_double(stmt, i, get_random_double());
            if (rc != SQLITE_OK){
                close_connection(driver, stmt, "error while binding param", rc);
            }
        }
    }
    return executer(driver,stmt);
}
int update(void *driver, int id){
    sqlite3_stmt *stmt;
    char query[MAX_SQL_LENGTH] = "UPDATE test_table SET (field1) = (?1) WHERE id=?2";
    prepare(driver, query, (void **)&stmt);
    sqlite3_bind_int(stmt, 2,get_random_database_index());
    int rc;
    int b = get_random_int();
    rc = sqlite3_bind_int(stmt, 1, b);
    if (rc != SQLITE_OK){
        close_connection(driver, stmt, "error while binding param", rc);
    }
    return executer(driver, stmt);
}

void perform_operation(double stats[], database_single_operation op){
    sqlite3 *db;
    int rc = sqlite3_open("study_databases.db", &db);
    if (rc) {
        fprintf(stderr, "Cannot open database: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    setUpDatabase(db);
    clock_t start, end;
    for (int i = 0; i < N_STATS; i++){
        start= clock();
            (*op)(db, i);
        end = clock();
        stats[i] = (double) (end - start) / CLOCKS_PER_SEC;
    }
    sqlite3_close(db);
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