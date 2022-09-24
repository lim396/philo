#include "philo.h"

void	eating(t_philo *philo)
{
	int		n;

	n = philo->config->num_of_philo;
	pthread_mutex_lock(&(philo->info->fork)[philo->id]);
	pthread_mutex_lock(&(philo->info->fork)[(philo->id + 1) % n]);
	print_status(TAKEN_A_FORK, philo);
	print_status(TAKEN_A_FORK, philo);
	print_status(EATINGN, philo);
	update_last_ate_time(philo);
	highly_accurate_usleep_in_ms(philo->config->time_to_eat, philo);
	philo->number_of_meals++;
	pthread_mutex_unlock(&(philo->info->fork)[philo->id]);
	pthread_mutex_unlock(&(philo->info->fork)[(philo->id + 1) % n]);
}

void	sleeping(t_philo *philo)
{
	print_status(SLEEPING, philo);
	highly_accurate_usleep_in_ms(philo->config->time_to_sleep, philo);
}

void	thinking(t_philo *philo)
{
	print_status(THINKING, philo);
}

void	waiting_to_die_with_a_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->info->fork)[philo->id]);
	print_status(TAKEN_A_FORK, philo);
	usleep(philo->config->time_to_die);
	pthread_mutex_unlock(&(philo->info->fork)[philo->id]);
	return ;
}

void	routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->config->num_of_philo == 1)
		return (waiting_to_die_with_a_fork(philo));
	if ((philo->id + 1) % 2 == 0)
		usleep(200);
	while (!check_end(philo))
	{
		eating(philo);
		if (philo->number_of_meals == philo->config->num_of_must_eat)
		{
			pthread_mutex_lock(philo->info->end_meals);
			philo->info->n_of_finish_eat++;
			pthread_mutex_unlock(philo->info->end_meals);
			return ;
		}
		sleeping(philo);
		thinking(philo);
	}
	return ;
}
