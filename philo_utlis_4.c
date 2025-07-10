/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:36:34 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/10 23:44:42 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_all(t_data *data)
{
	t_philos_data	*philo;
	t_forks_data	*forks;
	t_philos_data	*temp;
	t_forks_data	*tp;

	philo = data->philos;
	forks = data->forks;
	while (philo)
	{
		temp = philo;
		philo = philo->next;
		free(temp);
	}
	while (forks)
	{
		tp = forks;
		forks = forks->next;
		pthread_mutex_destroy(&tp->mutex);
		free(temp);
	}
	pthread_mutex_destroy(&data->mutex);
	pthread_mutex_destroy(&data->write_mutex);
}

int	parsing_helper(char *argv[], int *i)
{
	long	value;

	value = ft_atoi(argv[*i]);
	if (value == LONG_MAX || value <= 0)
		return (1);
	if (*i == 1 && value > 200)
		return (1);
	if (*i >= 2 && *i <= 4 && value < 60)
		return (1);
	return (0);
}
