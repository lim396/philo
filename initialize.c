#include "philo.h"

void	init_config(char **argv, t_config *config)
{
	config->num_of_philo = ft_atoi(argv[1]);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	config->num_of_must_eat = -1;
	if (argv[5])
		config->num_of_must_eat = ft_atoi(argv[5]);
}

int	init_overall_info(int n, t_overall *info)
{
	memset(info, 0, sizeof(*info));
	info->n_of_finish_eat = 0;
	info->end_simulation = 0;
	info->end_meals = (t_mutex *)malloc(sizeof(t_mutex));
	info->get_time_and_print = (t_mutex *)malloc(sizeof(t_mutex));
	info->end_flag = (t_mutex *)malloc(sizeof(t_mutex));
	info->fork = (t_mutex *)malloc(sizeof(t_mutex) * n);
	if (!(info->end_meals) || !(info->get_time_and_print)
		|| !(info->end_flag) || !(info->fork))
	{
		return (malloc_error_handler(0, info, NULL));
	}
	return (0);
}

int	init_philosophers(t_config *config, t_overall *info, t_philo **philo)
{
	int	i;
	int	n;

	n = config->num_of_philo;
	*philo = (t_philo *)ft_calloc(sizeof(t_philo), n);
	if (*philo == NULL)
		return (malloc_error_handler(0, info, philo));
	i = 0;
	while (i < n)
	{
		(*philo)[i].id = i;
		(*philo)[i].number_of_meals = 0;
		(*philo)[i].config = config;
		(*philo)[i].info = info;
		(*philo)[i].last_ate_time = 0;
		(*philo)[i].last_ate = (t_mutex *)malloc(sizeof(t_mutex));
		if ((*philo)[i].last_ate == NULL)
			return (malloc_error_handler(i, info, philo));
		i++;
	}
	return (0);
}

void	init_mutex(int n, t_overall *info, t_philo **philo)
{
	int	i;

	pthread_mutex_init(info->end_meals, NULL);
	pthread_mutex_init(info->get_time_and_print, NULL);
	pthread_mutex_init(info->end_flag, NULL);
	i = 0;
	while (i < n)
	{
		pthread_mutex_init(&(info->fork)[i], NULL);
		pthread_mutex_init((*philo)[i].last_ate, NULL);
		i++;
	}
}
