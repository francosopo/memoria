#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gen_data.h>

#define MAX_SQL_LENGTH 900
#define N_OPS 18

typedef void (*p_operation)(double*);
typedef int (*database_single_operation)(void*, int);

void setUpDatabase(void *driver);
void insert_mock_data_table2(void *driver);
void create_indexes(void *driver);
void prepare(void *driver, char *query, void **stmt);

int select(void *driver, int id);
int select_with_index(void *driver, int id);
int select_with_index_hash(void *driver, int id);
int select_from_where_inner_join(void *driver, int id);
int select_from_where_inner_join_with_index(void *driver, int id);
int select_from_where_inner_join_with_index_hash(void *driver, int id);
int select_from_where_outer_join(void *driver, int id);
int select_from_where_outer_join_with_index(void *driver, int id);
int select_from_where_outer_join_with_index_hash(void *driver, int id);
int select_from_where_right_join(void *driver, int id);
int select_from_where_right_join_with_index(void *driver, int id);
int select_from_where_right_join_with_index_hash(void *driver, int id);
int select_from_where_left_join(void *driver, int id);
int select_from_where_left_join_with_index(void *driver, int id);
int select_from_where_left_join_with_index_hash(void *driver, int id);
int insert(void *driver, int id);
int update(void *driver, int id);
int delete_from(void *driver, int id);

void perform_select(double stats[]);
void perform_select_with_index(double stats[]);
void perform_select_with_index_hash(double stats[]);
void perform_select_from_where_inner_join(double stats[]);
void perform_select_from_where_inner_join_with_index(double stats[]);
void perform_select_from_where_inner_join_with_index_hash(double stats[]);
void perform_select_from_where_outer_join(double stats[]);
void perform_select_from_where_outer_join_with_index(double stats[]);
void perform_select_from_where_outer_join_with_index_hash(double stats[]);
void perform_select_from_where_right_join(double stats[]);
void perform_select_from_where_right_join_with_index(double stats[]);
void perform_select_from_where_right_join_with_index_hash(double stats[]);
void perform_select_from_where_left_join(double stats[]);
void perform_select_from_where_left_join_with_index(double stats[]);
void perform_select_from_where_left_join_with_index_hash(double stats[]);
void perform_insert(double stats[]);
void perform_update(double stats[]);
void perform_delete_from(double stats[]);

void perform(FILE *out);

void get_filename(char *name, int length);