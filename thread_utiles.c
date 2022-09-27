#include "philo.h"

bool	check_end(t_philo *philo)
{
	pthread_mutex_lock(philo->info->end_flag);
	if (philo->info->end_simulation)
	{
		pthread_mutex_unlock(philo->info->end_flag);
		return (true);
	}
	pthread_mutex_unlock(philo->info->end_flag);
	return (false);
}

long	get_elapsed_time_in_ms(long start_time)
{
	struct timeval	now;
	long			elapsed_time;

	gettimeofday(&now, NULL);
	elapsed_time = (now.tv_sec * 1e3 + now.tv_usec * 1e-3) - start_time;
	return (elapsed_time);
}

void	print_status(int status, t_philo *philo)
{
	long	just_now;

	pthread_mutex_lock(philo->info->get_time_and_print);
	just_now = get_elapsed_time_in_ms(philo->config->start_time);
	if (status == TAKEN_A_FORK && !check_end(philo))
		printf("%ld %d has taken a fork\n", just_now, philo->id + 1);
	else if (status == EATINGN && !check_end(philo))
		printf("%ld %d is eating\n", just_now, philo->id + 1);
	else if (status == SLEEPING && !check_end(philo))
		printf("%ld %d is sleeping\n", just_now, philo->id + 1);
	else if (status == THINKING && !check_end(philo))
		printf("%ld %d is thinking\n", just_now, philo->id + 1);
	else if (status == DIE)
		printf("%ld %d died\n", just_now, philo->id + 1);
	pthread_mutex_unlock(philo->info->get_time_and_print);
}

void	update_last_ate_time(t_philo *philo)
{
	pthread_mutex_lock(philo->last_ate);
	philo->last_ate_time = get_elapsed_time_in_ms(philo->config->start_time);
	pthread_mutex_unlock(philo->last_ate);
}

//void	high_accuracy_usleep_in_ms(int sleep_time, t_philo *philo, int status)
void	high_accuracy_usleep_in_ms(int sleep_time, t_philo *philo, int flag)
{
	long	sleep_start_time;
	int		delay;
	
	delay = 0;
	sleep_start_time = get_elapsed_time_in_ms(philo->config->start_time);
//	if (status == EATINGN)
	delay = sleep_start_time - philo->ideal_time;
	while (get_elapsed_time_in_ms(philo->config->start_time) - sleep_start_time
		< sleep_time - delay)
		usleep(500);
//	if (status == EATINGN && philo->config->num_of_philo % 2 == 0)
//	{
//	philo->ideal_time += sleep_time + philo->config->time_to_sleep;
	if (flag)
		philo->ideal_time += sleep_time;
//	}
//	else if(status == EATINGN && philo->config->num_of_philo % 2 != 0)
//	{
//		if (sleep_time * 3 > sleep_time + philo->config->time_to_sleep)
//		{
//			philo->ideal_time += sleep_time * 3;
//		}
//		else
//			philo->ideal_time += sleep_time + philo->config->time_to_sleep;
//	}
}
