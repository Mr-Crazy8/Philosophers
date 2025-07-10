#include "philo.h"


void ft_usleep(long long time, t_philos_data *philos)
{
    long long start = time_in_ms();
    
    while ((time_in_ms() - start) < time && !check_sim(philos))
    {
            usleep(100);
    }
}

void print_status(t_philos_data *philos, const char *status)
{
    pthread_mutex_lock(&philos->data->mutex);
    pthread_mutex_lock(&philos->data->write_mutex);
    if (!philos->data->simulation_stop)
        printf("%lld %d %s\n", time_in_ms() - philos->data->start_time, philos->philos_index + 1, status);
    pthread_mutex_unlock(&philos->data->write_mutex);
    pthread_mutex_unlock(&philos->data->mutex);
}

void give_back_forks(t_philos_data *philos)
{
    if (philos->data->num_of_philos == 1)
            return;
    if (philos->philos_index % 2 == 0)
        {
            pthread_mutex_unlock(&philos->right_fork->mutex);
            pthread_mutex_unlock(&philos->left_fork->mutex);
        }
    else
        {
            pthread_mutex_unlock(&philos->left_fork->mutex);
            pthread_mutex_unlock(&philos->right_fork->mutex);
        }
}

int check_sim(t_philos_data *philos)
{
    int stop;
    pthread_mutex_lock(&philos->data->mutex);
    stop = philos->data->simulation_stop;
    pthread_mutex_unlock(&philos->data->mutex);
    return (stop);
}

void *monitor_task(void *arg)
{
    t_data *data = (t_data *)arg;

    while (1)
    {
        if (check_death(data) || check_meals_complete(data))
            break;
        usleep(1000);
    }
    return NULL;
}