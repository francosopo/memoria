#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gen_data.h>

#define MAX_SQL_LENGTH 900
#define N_OPS 3

typedef void (*p_operation)(double*);
typedef int (*database_single_operation)(void*, int);


void prepare(void *driver, char *query, void **stmt);

int select(void *driver, int id);
int insert(void *driver, int id);
int update(void *driver, int id);

void perform_select(double stats[]);
void perform_insert(double stats[]);
void perform_update(double stats[]);


void perform(FILE *out);

void get_filename(char *name, int length);