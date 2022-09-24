CC = cc
CFLAGS = -g -Wall -Wextra -Werror
NAME = philo
SRCS =	check_args.c \
		end_process.c \
		error_handler.c \
		initialize.c \
		main.c \
		monitor.c \
		routine.c \
		run_thread.c \
		thread_utiles.c \
		utiles.c \
 
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
