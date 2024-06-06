CC=gcc
INCLUDE_DIR=-I./include
CFLAGS= -Wall -pedantic -std=c99 -Werror -g
DBMS_FOLDER=./dbms
BENCHMARK_FILE=benchmark.c
PSQL_SRC=${DBMS_FOLDER}/psql/${BENCHMARK_FILE}
MARIADB_SRC=${DBMS_FOLDER}/mariadb/${BENCHMARK_FILE}
SQLITE_SRC=${DBMS_FOLDER}/sqlite/${BENCHMARK_FILE}

STATS_FILE=./stats/stats.c

MAIN_FILE=./code/study.c

data_source.o:
	${CC} ${CFLAGS} ${INCLUDE_DIR} -c ./data_source/src/gen_data.c -o data_source.o

stats.o:
	${CC} ${CFLAGS} ${INCLUDE_DIR} -c ${STATS_FILE} -o stats.o

psql.o:
	${CC} ${CFLAGS} ${INCLUDE_DIR} -I/usr/include/postgresql -c -o psql_obj ${PSQL_SRC} 

psql: stats.o psql.o data_source.o
	${CC} ${CFLAGS} ${INCLUDE_DIR} stats.o psql_obj data_source.o ${MAIN_FILE} -o psql_out -lgsl -lgslcblas -lm -lpq

sqlite.o:
	${CC} ${CFLAGS} ${INCLUDE_DIR} -c -o sqlite_obj ${SQLITE_SRC}

sqlite: sqlite.o stats.o data_source.o
	${CC} ${CFLAGS} ${INCLUDE_DIR} stats.o sqlite_obj data_source.o ${MAIN_FILE} -o sqlite_out -lgsl -lgslcblas -lm -lsqlite3

mariadb.o:
	${CC} ${CFLAGS} ${INCLUDE_DIR} -I/usr/local/mysql/include/mysql -c -o mariadb_obj ${MARIADB_SRC}

mariadb: stats.o mariadb.o data_source.o
	${CC} ${CFLAGS} ${INCLUDE_DIR} stats.o mariadb_obj data_source.o ${MAIN_FILE} -o mariadb_out -lmariadb -lgsl -lgslcblas -lm

clean:
	rm -f psql_obj
	rm -f sqlite_obj
	rm -f mariadb_obj
	rm -f psql_out
	rm -f sqlite_out
	rm -f mariadb_out
	rm -f stats.o