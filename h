./philo 5 200 100 100
gcc -Wall -Wextra -Werror -pthread philo.c -o philo

gcc -Wall -Wextra -Werror -pthread -g -fsanitize=thread philo.c -o philo

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]



