

#include "philo.h"


void *philo_life(void *arg)
{
    t_philos_data *philos;

    philos = (t_philos_data *)arg;
    if ((philos->philos_index + 1 )% 2 == 1)
        ft_usleep(philos->data->time_to_eat / 2 , philos);
    while (!check_sim(philos))
    {   
            think(philos);
        if (!check_sim(philos))
            eat(philos);
        if (!check_sim(philos))
            take_a_nap(philos);
    }

    return NULL;
}

int check_death(t_data *data)
{
    t_philos_data *tmp = data->philos;
    long long current_time;
    long long last_meal;

    while (tmp)
    {
        pthread_mutex_lock(&data->mutex);
        current_time = time_in_ms();
        last_meal = tmp->last_meal_time;
        
        if (current_time - last_meal > data->time_to_die)
        {
            pthread_mutex_unlock(&data->mutex);
            print_status(tmp, "died");
            data->simulation_stop = 1;
            return 1;
        }
        else
            pthread_mutex_unlock(&data->mutex);
        tmp = tmp->next;
    }
    return 0;
}

int check_meals_complete(t_data *data)
{
    if (data->meals_required == -1)
        return 0;

    t_philos_data *tmp = data->philos;
    int all_done = 1;

    pthread_mutex_lock(&data->mutex);
    while (tmp)
    {
        if (tmp->eat_count < data->meals_required)
        {
            all_done = 0;
            break;
        }
        tmp = tmp->next;
    }
    
    if (all_done)
        data->simulation_stop = 1;
    pthread_mutex_unlock(&data->mutex);
    
    return all_done;
}

int creat_thread(t_philos_data *philos)
{
    t_philos_data *tmp;
    int i = 0;
    pthread_t monitor_thread;
    tmp = philos;

    while (tmp)
    {
        i = pthread_create(&tmp->thread, NULL, philo_life, tmp);
        if (i != 0) 
        {
            printf("Thread creation failed\n");
            return (1);
        }
        tmp = tmp->next;
    }
    tmp = philos;
    if (tmp != NULL)
        i = pthread_create(&monitor_thread, NULL, monitor_task, tmp->data);
    if (i != 0) 
    {
            printf("Thread creation failed\n");
            return (1);
    }
    tmp = philos;
    while (tmp)
    {
        pthread_join(tmp->thread, NULL);
        tmp = tmp->next;
    }
    pthread_join(monitor_thread, NULL);
    return 0;
}
void clean_all(t_data *data)
{
    t_philos_data *philo;
    t_forks_data *forks;

    philo = data->philos;
    forks = data->forks;

    while(philo)
    {
        t_philos_data *temp = philo;
        philo = philo->next;
        free(temp);
    }
     while (forks)
    {
        t_forks_data *temp = forks;
        forks = forks->next;
        pthread_mutex_destroy(&temp->mutex);
        free(temp);
    }
    pthread_mutex_destroy(&data->mutex);
    pthread_mutex_destroy(&data->write_mutex);
}
int main(int argc, char *argv[])
{
    t_philos_data *philos_info = NULL;
    t_forks_data *forks = NULL;
    t_data data;
    
    if (parsing(argc, argv) == 1)
        return(printf("parsing error\n"));

    data_init(&data, argv);
    philo_init(&philos_info, ft_atoi(argv[1]), &data); 
    forks_init(&forks, ft_atoi(argv[1]));
    forks_assignment(philos_info, forks);
    data.forks = forks;
    data.philos = philos_info;
    creat_thread(philos_info);
    clean_all(&data);
    return 0;
}