#include <operations.h>
#include <stats.h>

void perform(FILE *out){
    p_operation ops[N_OPS] = {
            &perform_insert,
            &perform_select,
            &perform_select_with_index,
            &perform_select_with_index_hash,
            &perform_select_from_where_inner_join,
            &perform_select_from_where_inner_join_with_index,
            &perform_select_from_where_inner_join_with_index_hash,
            &perform_select_from_where_outer_join,
            &perform_select_from_where_outer_join_with_index,
            &perform_select_from_where_outer_join_with_index_hash,
            &perform_select_from_where_right_join,
            &perform_select_from_where_right_join_with_index,
            &perform_select_from_where_right_join_with_index_hash,
            &perform_select_from_where_left_join,
            &perform_select_from_where_left_join_with_index,
            &perform_select_from_where_left_join_with_index_hash,
            &perform_update,
            &perform_delete
};
    double data[N_STATS][N_OPS];
    for (int k = 0; k < N_OPS; k++){
        (*ops[k])(data[k]);
        double avg = the_avg(data[k], N_STATS);
        double std = the_std(data[k], N_STATS);
        double min = the_min(data[k], N_STATS);
        double max = the_max(data[k], N_STATS);
        double med = the_med(data[k], N_STATS);
        double var = the_var(data[k], N_STATS);
        fprintf(out, "%.9lf,%.9lf,%.9lf,%.9lf,%.9lf,%.9lf\n",avg, std,min,max,med,var);
    }
}

int main(int argc, char *argv[]){
    srand(55);
    char filename[100];
    get_filename(filename, 100);
    FILE *out = fopen(filename, "w");
    fprintf(out, "%s", "avg,std,min,max,med,var\n");
    perform(out);
    fclose(out);
}