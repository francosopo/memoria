#include <stats.h>

double the_avg(double arr[], int n){
    return gsl_stats_mean(arr, 1, n);
}
double the_std(double arr[], int n){
    return gsl_stats_sd(arr, 1, n);
}
double the_min(double arr[], int n){
    return gsl_stats_min(arr, 1, n);
}
double the_max(double arr[], int n){
    return gsl_stats_max(arr, 1, n);
}
double the_med(double arr[], int n){
    //avg case: O(n)
    //worst_case O(n**2)
    return gsl_stats_median(arr,1,n);
}
double the_var(double arr[], int n){
    return gsl_stats_variance(arr,1,n);
}