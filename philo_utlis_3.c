#include "philo.h"

void think(t_philos_data *philos)
{
        print_status(philos, "is thinking");
        if (philos->data->num_of_philos % 2 == 1 && philos->data->num_of_philos != 1)
        {
            // Dynamic thinking time based on number of philosophers
            long long think_time = 10;
            
            if (philos->data->num_of_philos > 20)
            {
                // For large numbers, use much longer thinking time to prevent monopolization
                // Make thinking time proportional to the contention level
                think_time = (philos->data->time_to_eat * philos->data->num_of_philos) / 20;
                // Add significant philosopher-specific variation to break patterns
                think_time += (philos->philos_index % 30);
                // Ensure minimum thinking time is substantial
                if (think_time < 50)
                    think_time = 50;
            }
            
            ft_usleep(think_time, philos);
        }
        philos->status = 2;
}

void take_fork(t_philos_data *philos)
{
     if (philos->data->num_of_philos == 1)
    {
        pthread_mutex_lock(&philos->left_fork->mutex);
        print_status(philos, "has taken a fork");
        ft_usleep(philos->data->time_to_die + 1, philos);
        pthread_mutex_unlock(&philos->left_fork->mutex);
        return;
    }

        if (philos->philos_index % 2 == 0)
        {
            pthread_mutex_lock(&philos->left_fork->mutex);
            print_status(philos, "has taken a fork");
                pthread_mutex_lock(&philos->right_fork->mutex);
                print_status(philos, "has taken a fork");
        }
        else if (philos->philos_index % 2 != 0)
        {
            pthread_mutex_lock(&philos->right_fork->mutex);
            print_status(philos, "has taken a fork");
            pthread_mutex_lock(&philos->left_fork->mutex);
            print_status(philos, "has taken a fork");
        
        }
}

void eat(t_philos_data *philos)
{
    if (philos->right_fork == NULL)
        ft_usleep(philos->data->time_to_die, philos);
    if (philos->status != 3)
    {
        pthread_mutex_lock(&philos->data->mutex);
        if (philos->data->simulation_stop != 1)
        {
            pthread_mutex_unlock(&philos->data->mutex);
            print_status(philos, "is eating");
            pthread_mutex_lock(&philos->data->mutex);
            philos->last_meal_time = time_in_ms();
            philos->eat_count += 1;
            pthread_mutex_unlock(&philos->data->mutex);
            
            philos->status = 0;
            ft_usleep(philos->data->time_to_eat, philos);
        }
    }
}

void take_a_nap(t_philos_data *philos)
{
    print_status(philos, "is sleeping");
    philos->status = 1;
    ft_usleep(philos->data->time_to_sleep, philos);
}

