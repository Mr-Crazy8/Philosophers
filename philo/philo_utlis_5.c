/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:07:55 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 21:25:19 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error_m(t_philos_data *tmp)
{
	t_philos_data	*tp;

	pthread_mutex_lock(&tmp->data->mutex);
	tmp->data->simulation_stop = 1;
	pthread_mutex_unlock(&tmp->data->mutex);
	tp = tmp;
	while (tp != NULL)
	{
		pthread_join(tp->thread, NULL);
		tp = tp->next;
	}
	printf("Monitor thread creation failed\n");
	return (1);
}

void	*philo_life(void *arg)
{
	t_philos_data	*philos;

	philos = (t_philos_data *)arg;
	if (check_sim(philos))
		return (NULL);
	if (philos->data->num_of_philos == 1)
	{
		think(philos);
		pthread_mutex_lock(&philos->left_fork->mutex);
		print_status(philos, "has taken a fork");
		ft_usleep(philos->data->time_to_die, philos);
		pthread_mutex_unlock(&philos->left_fork->mutex);
		return (NULL);
	}
	if ((philos->philos_index + 1) % 2 == 1)
		ft_usleep(philos->data->time_to_eat / 2, philos);
	while (!check_sim(philos))
	{
		think(philos);
		if (!check_sim(philos))
			eat(philos);
		if (!check_sim(philos))
			take_a_nap(philos);
	}
	return (NULL);
}
