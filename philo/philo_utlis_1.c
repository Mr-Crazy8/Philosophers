/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:00:23 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 20:42:30 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_init(t_philos_data **philos_info, int count_philo, t_data *data)
{
	int				i;
	t_philos_data	*new_node;

	i = 0;
	while (i < count_philo)
	{
		new_node = malloc(sizeof(t_philos_data));
		if (!new_node)
			return (1);
		new_node->next = NULL;
		new_node->philos_index = i;
		new_node->eat_count = 0;
		new_node->last_meal_time = data->start_time;
		new_node->left_fork = NULL;
		new_node->right_fork = NULL;
		new_node->data = data;
		add_philos_back(philos_info, new_node);
		i++;
	}
	return (0);
}

void	clean_error_forks(t_forks_data **forks)
{
	t_forks_data	*tmp;
	t_forks_data	*tp;

	tmp = *forks;
	while (tmp)
	{
		tp = tmp;
		tmp = tmp->next;
		free(tp);
	}
}

int	forks_init(t_forks_data **forks, int forks_count)
{
	t_forks_data	*new_node;
	int				i;

	i = 0;
	while (i < forks_count)
	{
		new_node = malloc(sizeof(t_forks_data));
		if (!new_node)
			return (clean_error_forks (forks), 1);
		new_node->next = NULL;
		new_node->fork_id = i;
		if (pthread_mutex_init(&new_node->mutex, NULL) != 0)
		{
			free(new_node);
			clean_error(forks, i);
			return (1);
		}
		add_forks_back(forks, new_node);
		i++;
	}
	return (0);
}

int	data_init(t_data *data, char *argv[])
{
	if (ft_atoi(argv[1]) != 0)
		data->num_of_philos = ft_atoi(argv[1]);
	else
	{
		printf("parsing error\n");
		return (1);
	}
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	// printf("data->time_to_die == [%ld]\n");
	if (argv[5] != NULL)
		data->meals_required = ft_atoi(argv[5]);
	else
		data->meals_required = -1;
	data->simulation_stop = 0;
	data->start_time = time_in_ms();
	data->forks = NULL;
	data->philos = NULL;
	if (pthread_mutex_init(&data->mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->write_mutex, NULL))
		return (pthread_mutex_destroy(&data->mutex), 1);
	return (0);
}

void	forks_assignment(t_philos_data *philos_info, t_forks_data *forks)
{
	t_philos_data	*tmp;
	t_forks_data	*forks_tmp;

	if (!philos_info || !forks)
		return ;
	tmp = philos_info;
	forks_tmp = forks;
	while (tmp)
	{
		tmp->left_fork = forks_tmp;
		if (tmp->data->num_of_philos == 1)
			tmp->right_fork = NULL;
		else
		{
			if (forks_tmp->next != NULL)
				tmp->right_fork = forks_tmp->next;
			else
				tmp->right_fork = forks;
		}
		forks_tmp = forks_tmp->next;
		tmp = tmp->next;
	}
}
