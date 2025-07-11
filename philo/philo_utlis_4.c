/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:36:34 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 20:46:37 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_all(t_data *data)
{
	t_philos_data	*philo;
	t_forks_data	*forks;
	t_forks_data	*tp;
	t_philos_data	*temp;

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
		free(tp);
	}
	pthread_mutex_destroy(&data->mutex);
	pthread_mutex_destroy(&data->write_mutex);
}

int	parsing_helper(char *argv[], int *i)
{
	long	value;

	value = ft_atoi(argv[*i]);
	printf("here  [%ld]\n", value);
	if (value >= LONG_MAX || value <= 0)
		return (1);
	// if (*i == 1 && value > 200)
	// 	return (1);
	// if (*i >= 2 && *i <= 4 && value < 60)
	// 	return (1);
	return (0);
}

void	clean_error_philo(t_philos_data	*philo)
{
	t_philos_data	*temp;

	while (philo)
	{
		temp = philo;
		philo = philo->next;
		free(temp);
	}
}

void	clean_error(t_forks_data **forks, int start)
{
	int				i;
	t_forks_data	*tp;
	t_forks_data	*tmp;

	tp = *forks;
	i = 0;
	while (i < start)
	{
		pthread_mutex_destroy(&tp->mutex);
		tp = tp->next;
		i++;
	}
	tmp = *forks;
	while (tmp)
	{
		tp = tmp;
		tmp = tmp->next;
		free(tp);
	}
}

long long	time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
