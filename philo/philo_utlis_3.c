/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:09:56 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 19:08:20 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think(t_philos_data *philos)
{
	print_status(philos, "is thinking");
	if (philos->data->num_of_philos % 2 == 1)
		ft_usleep(10, philos);
}

void	take_fork(t_philos_data *philos)
{
	if (philos->data->num_of_philos == 1)
	{
		pthread_mutex_lock(&philos->left_fork->mutex);
		print_status(philos, "has taken a fork");
		ft_usleep(philos->data->time_to_die + 1, philos);
		pthread_mutex_unlock(&philos->left_fork->mutex);
		return ;
	}
	if (philos->philos_index % 2 == 0)
	{
		pthread_mutex_lock(&philos->left_fork->mutex);
		print_status(philos, "has taken a fork");
		pthread_mutex_lock(&philos->right_fork->mutex);
		print_status(philos, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philos->right_fork->mutex);
		print_status(philos, "has taken a fork");
		pthread_mutex_lock(&philos->left_fork->mutex);
		print_status(philos, "has taken a fork");
	}
}

void	eat(t_philos_data *philos)
{
	take_fork(philos);
	if (philos->right_fork == NULL)
		ft_usleep(philos->data->time_to_die, philos);
	else
	{
		pthread_mutex_lock(&philos->data->mutex);
		philos->last_meal_time = time_in_ms();
		philos->eat_count += 1;
		pthread_mutex_unlock(&philos->data->mutex);
		print_status(philos, "is eating");
		ft_usleep(philos->data->time_to_eat, philos);
		give_back_forks(philos);
	}
}

void	take_a_nap(t_philos_data *philos)
{
	print_status(philos, "is sleeping");
	ft_usleep(philos->data->time_to_sleep, philos);
}

int	print_error(int j, t_philos_data *tmp)
{
	int				i;
	t_philos_data	*tp;

	tp = tmp;
	i = 0;
	pthread_mutex_lock(&tp->data->mutex);
	tp->data->simulation_stop = 1;
	pthread_mutex_unlock(&tp->data->mutex);
	tp = tmp;
	while (i < j && tp != NULL)
	{
		pthread_join(tp->thread, NULL);
		tp = tp->next;
		i++;
	}
	printf("Thread creation failed\n");
	return (1);
}
