

#include "philo.h"


static int	ft_space(const char **str, int sign)
{
	while ((**str >= 9 && **str <= 13) || **str == 32)
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign *= -1;
		(*str)++;
	}
	return (sign);
}

long	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	res;

	sign = 1;
	i = 0;
	res = 0;
	sign = ft_space(&str, sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1)
		{
			if (res > (LONG_MAX - (str[i] - '0')) / 10)
				return (LONG_MAX);
		}
		else
		{
			if (res > (LONG_MAX - (str[i] - '0')) / 10)
				return (LONG_MIN);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}


int  parsing(int argc, char *argv[])
{
    if(argc != 5 && argc != 6)
       return(1);

    int i = 1;
    int j = 0;

   while (argv && argv[i])
   {
        j = 0;
        while (argv[i][j])
        {
           if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
                return(1);
            j++;
        }
        i++;
   }
    return 0;
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


void philo_init(t_philos_data **philos_info, int count_philo, t_data *data)
{
    int i;
    t_philos_data *new_node;
    i = 0;
    
    while (i < count_philo)
    {
        new_node = malloc(sizeof(t_philos_data));
        if (!new_node) 
            return;
        new_node->next = NULL;
        new_node->philos_index = i;
        new_node->eat_count = 0;
        new_node->last_meal_time = data->start_time;
        new_node->left_fork = NULL;
        new_node->right_fork = NULL;
        new_node->data = data;
        new_node->status = -1;
        add_philos_back(philos_info, new_node);
        i++;
    }
}

void forks_init(t_forks_data **forks, int forks_count)
{
    t_forks_data *new_node;
    int i = 0;
    
    while (i < forks_count)
    {
        new_node = malloc(sizeof(t_forks_data));
        if (!new_node)
            return;
        new_node->next = NULL;
        new_node->fork_id = i;
        pthread_mutex_init(&new_node->mutex, NULL);
        add_forks_back(forks, new_node);
        i++;
    }
}


void data_init(t_data *data, char *argv[])
{
    data->num_of_philos = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    if (argv[5] != NULL)
        data->meals_required = ft_atoi(argv[5]);
    else
        data->meals_required = -1;
    data->simulation_stop = 0;
    data->start_time = time_in_ms();
    data->forks = NULL;
    data->philos = NULL;
    pthread_mutex_init(&data->mutex, NULL);
    pthread_mutex_init(&data->write_mutex, NULL);
}
void forks_assignment(t_philos_data *philos_info, t_forks_data *forks)
{
    t_philos_data *tmp;
    t_forks_data *forks_tmp;

    if (!philos_info || !forks)
        return;

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

long long time_in_ms()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
void think(t_philos_data *philos)
{
    if (philos->data->philos->status != 3)
    {
        pthread_mutex_lock(&philos->data->write_mutex);
        printf("[%lld] %d is thinking\n", time_in_ms() - philos->data->start_time , philos->philos_index + 1);
        pthread_mutex_unlock(&philos->data->write_mutex);
        philos->status = 2;
    }
}


void take_fork(t_philos_data *philos)
{

    if (philos->data->philos->status != 3)
    {
        if (philos->philos_index % 2 == 0)
        {
            pthread_mutex_lock(&philos->left_fork->mutex);
            pthread_mutex_lock(&philos->data->write_mutex);
            printf("[%lld] %d  has taken a fork\n", time_in_ms() - philos->data->start_time , philos->philos_index+1);
            pthread_mutex_unlock(&philos->data->write_mutex);
            if (philos->right_fork)
            {
                pthread_mutex_lock(&philos->right_fork->mutex);
                pthread_mutex_lock(&philos->data->write_mutex);
                printf("[%lld]  %d  has taken a fork\n", time_in_ms() - philos->data->start_time , philos->philos_index+1);
                pthread_mutex_unlock(&philos->data->write_mutex);
            }
            
        }
        else if (philos->philos_index % 2 != 0)
        {
            pthread_mutex_lock(&philos->right_fork->mutex);
            pthread_mutex_lock(&philos->data->write_mutex);
            printf("[%lld] %d  has taken a fork\n", time_in_ms() - philos->data->start_time , philos->philos_index+1);
            pthread_mutex_unlock(&philos->data->write_mutex);
            pthread_mutex_lock(&philos->left_fork->mutex);
            pthread_mutex_lock(&philos->data->write_mutex);
            printf("[%lld] %d  has taken a fork\n", time_in_ms() - philos->data->start_time , philos->philos_index+1);
            pthread_mutex_unlock(&philos->data->write_mutex);
        
        }
    }
}

void give_back_forks(t_philos_data *philos)
{
   
    if (philos->philos_index % 2 == 0)
        {
            if (philos->right_fork)
                pthread_mutex_unlock(&philos->right_fork->mutex);
            if (philos->left_fork)
                pthread_mutex_unlock(&philos->left_fork->mutex);
        }
    else if (philos->philos_index % 2 != 0)
        {
            if (philos->left_fork)
                pthread_mutex_unlock(&philos->left_fork->mutex);
            if (philos->right_fork)
                pthread_mutex_unlock(&philos->right_fork->mutex);
        }
}
void eat(t_philos_data *philos)
{
      if (philos->data->philos->status != 3 )
    {

        if (philos->left_fork  != NULL  && philos->right_fork != NULL)
        {
        pthread_mutex_lock(&philos->data->write_mutex);
        printf("[%lld]  %d   is eating\n", time_in_ms() - philos->data->start_time , philos->philos_index+1);
        pthread_mutex_unlock(&philos->data->write_mutex); 
        philos->eat_count += 1;
        philos->last_meal_time = time_in_ms();
        philos->status = 0;
        usleep(philos->data->time_to_eat * 1000);

        }
    }

}

void take_a_nap(t_philos_data *philos)
{

        pthread_mutex_lock(&philos->data->write_mutex);
        printf("[%lld]  %d    is sleeping\n", time_in_ms() - philos->data->start_time , philos->philos_index+1);
        pthread_mutex_unlock(&philos->data->write_mutex);
        philos->status = 1;
        usleep(philos->data->time_to_sleep * 1000);

}

int check_sim(t_philos_data *philos)
{
    int i;

    pthread_mutex_lock(&philos->data->mutex);
    i = philos->data->simulation_stop;
    pthread_mutex_unlock(&philos->data->mutex);
    return (i);
}

void *philo_life(void *arg)
{
    t_philos_data *philos;

    philos = (t_philos_data *)arg;

    while (!check_sim(philos))
    {   
        if (philos->status == 3)
            break;
        think(philos);

        take_fork(philos);

        eat(philos);

        give_back_forks(philos);

        take_a_nap(philos);
    }

    return NULL;
}


int check_death(t_data *data)
{
    t_philos_data *tmp = data->philos;
    long long current_time = time_in_ms();

    while (tmp)
    {
        pthread_mutex_lock(&data->mutex);
        if (current_time - tmp->last_meal_time > data->time_to_die)
        {
            data->simulation_stop = 1;
            pthread_mutex_unlock(&data->mutex);
            
            pthread_mutex_lock(&data->write_mutex);
            printf("[%lld] %d died\n", current_time - data->start_time, 
                   tmp->philos_index + 1);
            pthread_mutex_unlock(&data->write_mutex);
            return 1;
        }
        pthread_mutex_unlock(&data->mutex);
        tmp = tmp->next;
    }
    return 0;
}

int check_meals_complete(t_data *data)
{
    if (data->meals_required == -1)
        return 0;

    t_philos_data *tmp = data->philos;
    int all_done = 1;

    pthread_mutex_lock(&data->mutex);
    while (tmp)
    {
        if (tmp->eat_count < data->meals_required)
        {
            all_done = 0;
            break;
        }
        tmp = tmp->next;
    }
    
    if (all_done)
        data->simulation_stop = 1;
    pthread_mutex_unlock(&data->mutex);
    
    return all_done;
}

void *monitor_task(void *arg)
{
    t_data *data = (t_data *)arg;

    while (1)
    {
        if (check_death(data) || check_meals_complete(data))
            break;
            
        usleep(1); // Check every 1ms
    }
    
    return NULL;
}


// void *monitor_task(void *arg)
// {
//     t_data *data = (t_data *)arg;
//     t_philos_data *tmp;

//     tmp = data->philos;

//         while (tmp)
//         {
//             // if (data->meals_required != -1 && data->meals_required == data->philos->eat_count)
//             //     data->simulation_stop = 1;


            
//             if (time_in_ms() - tmp->last_meal_time > data->time_to_die)
//             {
//                     pthread_mutex_lock(&data->write_mutex);
//                     printf("[%lld]  %d  died\n", time_in_ms() - data->start_time , tmp->philos_index+1);
//                     pthread_mutex_unlock(&data->write_mutex);
//                     data->philos->status = 3;
//                     data->simulation_stop = 1;
//                     break;
//             }
//             if (data->simulation_stop == 1)
//                 break;
//             tmp = tmp->next;
//             if (tmp ==  NULL)
//                 tmp = data->philos;
//         }
//     return NULL;

// }

int creat_thread(t_philos_data *philos)
{
    t_philos_data *tmp;
    int i = 10;
    pthread_t monitor_thread;
    tmp = philos;

    while (tmp)
    {
        i = pthread_create(&tmp->thread, NULL, philo_life, tmp);
        if (i != 0) 
        {
            printf("Thread creation failed\n");
            return (1);
        }
        tmp = tmp->next;
    }

    tmp = philos;
    i = pthread_create(&monitor_thread, NULL, monitor_task, tmp->data);
    if (i != 0) 
    {
            printf("Thread creation failed\n");
            return (1);
    }
    tmp = philos;
    while (tmp)
    {
        pthread_join(tmp->thread, NULL);
        tmp = tmp->next;
    }
    pthread_join(monitor_thread, NULL);
    return 0;
}

int main(int argc, char *argv[])
{
    t_philos_data *philos_info = NULL;
    t_forks_data *forks = NULL;
    t_data data;
    
    if (parsing(argc, argv) == 1)
        return(printf("parsing error\n"));
    
    data_init(&data, argv);
    philo_init(&philos_info, ft_atoi(argv[1]), &data); 
    forks_init(&forks, ft_atoi(argv[1]));
    forks_assignment(philos_info, forks);
    data.forks = forks;
    data.philos = philos_info;
    creat_thread(philos_info);
    
    return 0;
}