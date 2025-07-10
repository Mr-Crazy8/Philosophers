/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 22:50:44 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/10 23:44:02 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(char *str)
{
	int		i;
	int		signe;
	long	res;

	i = 0;
	signe = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signe *= -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		return (LONG_MAX);
	return (res * signe);
}

int	parsing(int argc, char *argv[])
{
	int		i;
	int		j;

	if (argc != 5 && argc != 6)
		return (1);
	i = 1;
	j = 0;
	while (argv && argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (1);
			j++;
		}
		if (parsing_helper(argv, &i) == 1)
			return (1);
		i++;
	}
	return (0);
}

void	add_philos_back(t_philos_data **lst, t_philos_data *new)
{
	t_philos_data	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	add_forks_back(t_forks_data **lst, t_forks_data *new)
{
	t_forks_data	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	*philo_life(void *arg)
{
	t_philos_data	*philos;

	philos = (t_philos_data *)arg;
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
