/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:54 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:56 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		printf("%ld %d has taken a fork\n", philo->ideal_time, philo->id + 1);
	else if (status == EATINGN && !check_end(philo))
		printf("%ld %d is eating\n", philo->ideal_time, philo->id + 1);
	else if (status == SLEEPING && !check_end(philo))
		printf("%ld %d is sleeping\n", philo->ideal_time, philo->id + 1);
	else if (status == THINKING && !check_end(philo))
		printf("%ld %d is thinking\n", philo->ideal_time, philo->id + 1);
	else if (status == DIE)
		printf("%ld %d died\n", just_now, philo->id + 1);
	pthread_mutex_unlock(philo->info->get_time_and_print);
}

void	update_last_ate_time(t_philo *philo)
{
	pthread_mutex_lock(philo->last_ate);
	philo->last_ate_time = philo->ideal_time;
//	philo->last_ate_time = get_elapsed_time_in_ms(philo->config->start_time);
	pthread_mutex_unlock(philo->last_ate);
}

void	high_accuracy_usleep_in_ms(long sleep_time, t_philo *philo)
{
	long	sleep_start_time;
	long	delay;

	delay = 0;
	sleep_start_time = get_elapsed_time_in_ms(philo->config->start_time);
	delay = sleep_start_time - philo->ideal_time;
	while (get_elapsed_time_in_ms(philo->config->start_time) - sleep_start_time
		< sleep_time - delay)
		usleep(500);
	philo->ideal_time += sleep_time;
}
