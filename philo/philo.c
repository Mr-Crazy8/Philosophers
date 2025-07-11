/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 22:36:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 19:04:29 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_data *data)
{
	t_philos_data	*tmp;
	long long		current_time;
	long long		last_meal;

	tmp = data->philos;
	while (tmp)
	{
		pthread_mutex_lock(&data->mutex);
		current_time = time_in_ms();
		last_meal = tmp->last_meal_time;
		if (current_time - last_meal > data->time_to_die)
		{
			data->simulation_stop = 1;
			pthread_mutex_lock(&data->write_mutex);
			printf("%lld %d died\n", current_time - data->start_time,
				tmp->philos_index + 1);
			pthread_mutex_unlock(&data->write_mutex);
			pthread_mutex_unlock(&data->mutex);
			return (1);
		}
		else
			pthread_mutex_unlock(&data->mutex);
		tmp = tmp->next;
	}
	return (0);
}

int	check_meals_complete(t_data *data)
{
	t_philos_data	*tmp;
	int				all_done;

	if (data->meals_required == -1)
		return (0);
	tmp = data->philos;
	all_done = 1;
	pthread_mutex_lock(&data->mutex);
	while (tmp)
	{
		if (tmp->eat_count < data->meals_required)
		{
			all_done = 0;
			break ;
		}
		tmp = tmp->next;
	}
	if (all_done)
		data->simulation_stop = 1;
	pthread_mutex_unlock(&data->mutex);
	return (all_done);
}

int	join_all_threads(t_philos_data *philos, pthread_t monitor_thread)
{
	t_philos_data	*tmp;

	tmp = philos;
	while (tmp)
	{
		pthread_join(tmp->thread, NULL);
		tmp = tmp->next;
	}
	pthread_join(monitor_thread, NULL);
	return (0);
}

int	creat_thread(t_philos_data *philos)
{
	t_philos_data	*tmp;
	int				i;
	int				j;
	pthread_t		monitor_thread;

	tmp = philos;
	i = 0;
	j = 0;
	while (tmp)
	{
		i = pthread_create(&tmp->thread, NULL, philo_life, tmp);
		if (i != 0)
			return (print_error(j, tmp));
		tmp = tmp->next;
		j++;
	}
	tmp = philos;
	if (tmp != NULL)
		i = pthread_create(&monitor_thread, NULL, monitor_task, tmp->data);
	if (i != 0)
		return (print_error_m(tmp));
	join_all_threads(philos, monitor_thread);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philos_data	*philos_info;
	t_forks_data	*forks;
	t_data			data;

	if (parsing(argc, argv) == 1)
		return (printf("parsing error\n"));
	philos_info = NULL;
	forks = NULL;
	if (data_init(&data, argv) == 1)
		return (1);
	if (philo_init(&philos_info, ft_atoi(argv[1]), &data))
		return (clean_error_philo(philos_info), 1);
	if (forks_init(&forks, ft_atoi(argv[1])) == 1)
	{
		clean_error_philo(philos_info);
		return (1);
	}
	forks_assignment(philos_info, forks);
	data.forks = forks;
	data.philos = philos_info;
	if (creat_thread(philos_info) == 1)
		return (1);
	clean_all(&data);
	return (0);
}
