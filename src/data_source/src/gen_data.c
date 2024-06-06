#include <gen_data.h>

double get_random_double(){
    double res = (double) rand() / RAND_MAX;
    return res;
}

int get_random_int(){
    return rand();
}

int get_random_database_index(){
    int data = (int) (get_random_double() * N_STATS);
    return data;
}