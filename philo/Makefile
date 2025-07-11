CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = philo_utlis_0.c philo_utlis_1.c philo_utlis_2.c philo_utlis_3.c  philo_utlis_4.c philo_utlis_5.c philo.c
OBJS = $(SRCS:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean