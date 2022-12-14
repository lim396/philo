/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:36 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:38 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
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
	long		ideal_time;
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

bool	check_args(int argc, char **argv);

int		malloc_error_handler(int n, t_overall *info, t_philo **philo);
int		thread_error_handler(t_philo *philo, int flag);
void	print_args_error(void);

void	init_config(char **argv, t_config *config);
int		init_overall_info(int n, t_overall *info);
int		init_philosophers(t_config *config, t_overall *info, t_philo **philo);
void	init_mutex(int n, t_overall *info, t_philo **philo);

long	get_elapsed_time_in_ms(long start_time);
void	high_accuracy_usleep_in_ms(long sleep_time, t_philo *philo);
bool	check_end(t_philo *philo);
void	print_status(int status, t_philo *philo);
void	update_last_ate_time(t_philo *philo);

void	routine(void *arg);

void	monitor(void *arg);

int		thread_processing(int n, t_philo *philo);

void	destroy_mutex(int n, t_overall *info, t_philo **philo);

#endif
