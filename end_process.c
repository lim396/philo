#include "philo.h"

void	destroy_mutex(int n, t_overall *info, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy((*philo)[i].last_ate);
		free((*philo)[i].last_ate);
		pthread_mutex_destroy(&(info->fork)[i]);
		i++;
	}
	free(info->fork);
	pthread_mutex_destroy(info->end_meals);
	pthread_mutex_destroy(info->get_time_and_print);
	pthread_mutex_destroy(info->end_flag);
	free(info->end_meals);
	free(info->get_time_and_print);
	free(info->end_flag);
	free(*philo);
}
