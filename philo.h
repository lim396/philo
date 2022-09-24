#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

# define TAKEN_A_FORK 0
# define EATINGN 1
# define SLEEPING 2
# define THINKING 3
# define DIE 4

# define MONITOR_ERROR 8
# define PHILO_ERROR 16

typedef pthread_mutex_t		t_mutex;

typedef struct s_config		t_config;
struct s_config
{
	int		num_of_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_of_must_eat;
	long	start_time;
};

typedef struct s_overall	t_overall;
struct s_overall
{
	t_mutex	*fork;
	t_mutex	*end_meals;
	int		n_of_finish_eat;
	t_mutex	*get_time_and_print;
	t_mutex	*end_flag;
	bool	end_simulation;
};

typedef struct s_philo		t_philo;
struct s_philo
{
	int			id;
	int			number_of_meals;
	long		last_ate_time;
	t_mutex		*last_ate;
	pthread_t	thread;
	t_overall	*info;
	t_config	*config;
};

int		ft_isdigit(int c);
size_t	ft_strlen(const char *str);
void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);

bool	is_natural_num_ex_zero(const char *str);
bool	check_args(int argc, char **argv);

int		malloc_error_handler(int n, t_overall *info, t_philo **philo);
int		thread_error_handler(t_philo *philo, int flag);

void	init_config(char **argv, t_config *config);
int		init_overall_info(int n, t_overall *info);
int		init_philosophers(t_config *config, t_overall *info, t_philo **philo);
void	init_mutex(int n, t_overall *info, t_philo **philo);

long	get_elapsed_time_in_ms(long start_time);
void	highly_accurate_usleep_in_ms(int sleep_time, t_philo *philo);
bool	check_end(t_philo *philo);
void	print_status(int status, t_philo *philo);
void	update_last_ate_time(t_philo *philo);

void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo);
void	waiting_to_die_with_a_fork(t_philo *philo);
void	routine(void *arg);

int		check_died(int i, t_philo *philo);
void	big_brother(void *arg);

int		thread_processing(int n, t_philo *philo);

void	destroy_mutex(int n, t_overall *info, t_philo **philo);

#endif
