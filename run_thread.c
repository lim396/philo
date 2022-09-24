#include "philo.h"

int	thread_processing(int n, t_philo *philo)
{
	int			i;
	int			j;
	pthread_t	monitor;
	int			error_flag;

	error_flag = 0;
	i = -1;
	while (++i < n)
	{
		if (pthread_create(&philo[i].thread, NULL, (void *)routine, &philo[i]))
		{
			error_flag |= thread_error_handler(&philo[i], PHILO_ERROR);
			break ;
		}
	}
	if (pthread_create(&monitor, NULL, (void *)big_brother, philo))
		error_flag |= thread_error_handler(philo, MONITOR_ERROR);
	j = 0;
	while (j < i)
		pthread_join(philo[j++].thread, NULL);
	if (!(error_flag & MONITOR_ERROR))
		pthread_join(monitor, NULL);
	if (error_flag > 0)
		return (1);
	return (0);
}
