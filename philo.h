#ifndef PHILO_H
# define PHILO_H


#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include <limits.h>

typedef struct s_data
{
    int num_of_philos;         
    int time_to_die;          
    int time_to_eat;          
    int time_to_sleep;         
    int meals_required;      
    int simulation_stop;         
    long start_time;             
    pthread_mutex_t write_mutex; 
    t_forks_data *forks;      
    t_philos_data *philos;      

} t_data;

typedef struct s_forks_data
{
    int fork_id;
    pthread_mutex_t mutex;

    struct s_forks_data *next;

} t_forks_data;



typedef struct s_philos_data
{
    int philos_index; ///id
    int eat_count; /// meal count
    long last_meal_time; //last meal time
    pthread_t *thread;   //thread
    t_forks_data *left_fork;
    t_forks_data *right_fork;

    t_data *shared_data; 

    struct s_philos_data *next;

} t_philos_data;




#endif