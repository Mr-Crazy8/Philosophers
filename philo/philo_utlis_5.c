/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utlis_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:07:55 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 19:08:24 by anel-men         ###   ########.fr       */
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
