#include <operations.h>
#include <libpq-fe.h>


void get_filename(char *name, int length){
    snprintf(name, length, "psql.csv");
}

void close_connection(void *driver, PGresult *res, ExecStatusType expectedResult, ExecStatusType gotResult, char *msg){
    PGconn *conn = (PGconn*) driver;
    char *errMsg = PQresultErrorMessage(res);
    if (expectedResult != gotResult){
        fprintf(stderr, "PSQL ERROR: %s, %s\n",msg, errMsg);
        PQclear(res);
        PQfinish(conn);
        exit(-1);
    }
}

void setUpDatabase(void *driver){
    PGconn *conn = (PGconn*) driver;
    PGresult *res = PQexec(conn, "DROP TABLE IF EXISTS test_table");
    ExecStatusType got = PQresultStatus(res);
    close_connection(conn, res, PGRES_COMMAND_OK, got, "drop table");
    PQclear(res);
    PGresult *res2 = PQexec(conn, "CREATE TABLE IF NOT EXISTS test_table(id SERIAL, field1 INTEGER, field2 DOUBLE PRECISION, field3 INTEGER, field4 DOUBLE PRECISION, field5 INTEGER, field6 DOUBLE PRECISION)");
    ExecStatusType got2 = PQresultStatus(res2);
    close_connection(conn, res2, PGRES_COMMAND_OK, got2, "create table");
    PQclear(res2);
}

int executer(void *driver, char *query, int nParams,const Oid *paramTypes, const char *const *paramValues,const int *paramLengths, const int *paramFormats, int resFormat, ExecStatusType expectedResult, char *msg){ // char *query, ExecStatusType expectedResult){
    PGconn *conn = (PGconn*) driver;
    PGresult *res = PQexecParams(conn,query, nParams, paramTypes, paramValues, paramLengths, paramFormats, resFormat );
    ExecStatusType resultType = PQresultStatus(res);
    close_connection(conn, res, expectedResult, resultType, msg);
    PQclear(res);
    return 0;
}

int select(void *driver, int id){
    //PGconn *conn = (PGconn*) driver;
    //char *stmtName = "select_query";
    char *query = "SELECT * FROM test_table WHERE id=$1 LIMIT 1";
    const Oid paramTypes[1] = {23};// 23 is oid for int
    char theId[30];
    const int paramLengths[1] = {4};
    snprintf(theId, 30, "%i", id);
    const char * const inputs[1] = {theId};
    return executer(driver, query,1,paramTypes, inputs, paramLengths,NULL,0, PGRES_TUPLES_OK, "select");
}
int insert(void *driver, int id){
    //PGconn *conn = (PGconn*) driver;
    //char *stmtName = "insert_query";
    char *query = "INSERT INTO test_table(field1, field2, field3, field4, field5, field6) VALUES($1,$2,$3,$4,$5,$6)";
    const Oid paramTypes[6] = {23,701,23,701,23,701};
    char theNumbers[6][10];
    for (int i = 0; i < 6; i++){
        if ((i & 1) == 0){
            snprintf(theNumbers[i],10, "%i", get_random_int());
            //fprintf(stdout, "%s\n", theNumbers[i]);
        }else{
            snprintf(theNumbers[i],10, "%f", get_random_double());
            //fprintf(stdout, "%s\n", theNumbers[i]);
        }
    }
    const char * const inputs[6] = {theNumbers[0], theNumbers[1], theNumbers[2], theNumbers[3], theNumbers[4], theNumbers[5]};
    const int paramLengths[6] = {4,8,4,8,4,8};
    return executer(driver,query,6,paramTypes, inputs, paramLengths, NULL, 0, PGRES_COMMAND_OK, "insert");
}
int update(void *driver, int id){
    //PGconn *conn = (PGconn*) driver;
    //char *stmtName = "update_query";
    char *query = "UPDATE test_table SET field1 =$1 WHERE id=$2";
    const Oid paramTypes[2] = {23,23};
    char theNumbers[2][10];
    const int paramLengths[7] = {4,4};
    snprintf(theNumbers[1], 10, "%i", get_random_database_index());
    snprintf(theNumbers[0], 10, "%i", get_random_int());
    const char * const inputs[2] = {theNumbers[0], theNumbers[1]};
    return executer(driver, query,2,paramTypes, inputs, paramLengths,NULL,0, PGRES_COMMAND_OK, "update");
}
/*int del_operation(void *driver){
    return executer(driver, "DELETE FROM test_table", PGRES_COMMAND_OK);
}*/

void perform_operation(double stats[], database_single_operation op){
    PGconn *conn = PQconnectdb("host=localhost port=5432 dbname=study_databases user=study_user password=chaochao33356 sslmode=allow");
    ConnStatusType status = PQstatus(conn);

    if (status == CONNECTION_OK){
        setUpDatabase(conn);
        //perform operation
        clock_t start, end;
        
        for (int i = 0; i < N_STATS; i++){
            start = clock();
                (*op)(conn, i);
            end = clock();
            stats[i] = (double)(end - start) / CLOCKS_PER_SEC;
        }
    }
    PQfinish(conn);
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