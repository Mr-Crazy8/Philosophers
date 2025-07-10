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
		long value = ft_atoi(argv[i]);
        if (value == LONG_MAX || value <= 0)
            return 1;
		
		 if (i == 1 && value > 200) // Too many philosophers
            return 1;
        if (i >= 2 && i <= 4 && value < 60) // Times too short
            return 1;
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