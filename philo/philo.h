/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:12:33 by anel-men          #+#    #+#             */
/*   Updated: 2025/07/11 21:12:44 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_forks_data
{
	int					fork_id;
	pthread_mutex_t		mutex;
	struct s_forks_data	*next;
}	t_forks_data;

typedef struct s_data
{
	long					num_of_philos;
	long					time_to_die;
	long					time_to_eat;
	long					time_to_sleep;
	long					meals_required;
	int						simulation_stop;
	long long				start_time;
	pthread_mutex_t			write_mutex;
	pthread_mutex_t			mutex;
	t_forks_data			*forks;
	struct s_philos_data	*philos;
}	t_data;

typedef struct s_philos_data
{
	int						philos_index;
	int						eat_count;
	long long				last_meal_time;
	pthread_t				thread;
	t_forks_data			*left_fork;
	t_forks_data			*right_fork;
	t_data					*data;
	struct s_philos_data	*next;
}	t_philos_data;

int			check_death(t_data *data);
long		ft_atoi(char *str);
int			parsing(int argc, char *argv[]);
void		add_philos_back(t_philos_data **lst, t_philos_data *new);
void		add_forks_back(t_forks_data **lst, t_forks_data *new);
int			philo_init(t_philos_data **philos_info,
				int count_philo, t_data *data);
int			forks_init(t_forks_data **forks, int forks_count);
int			data_init(t_data *data, char *argv[]);
void		forks_assignment(t_philos_data *philos_info, t_forks_data *forks);
long long	time_in_ms(void);
void		think(t_philos_data *philos);
void		*philo_life(void *arg);
int			creat_thread(t_philos_data *philos);
int			check_sim(t_philos_data *philos);
void		print_status(t_philos_data *philos, const char *status);
void		ft_usleep(long long time, t_philos_data *philos);
void		*monitor_task(void *arg);
int			check_meals_complete(t_data *data);
void		take_fork(t_philos_data *philos);
void		eat(t_philos_data *philos);
void		give_back_forks(t_philos_data *philos);
void		take_a_nap(t_philos_data *philos);
int			parsing_helper(char *argv[], int *i);
int			print_error(int j, t_philos_data *tmp);
void		clean_all(t_data *data);
int			print_error_m(t_philos_data *tmp);
void		clean_error_philo(t_philos_data	*philo);
void		clean_error(t_forks_data **forks, int start);
int			print_error_m(t_philos_data *tmp);
#endif
