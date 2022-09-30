/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:42 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:44 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eating(t_philo *philo)
{
	int		n;

	n = philo->config->num_of_philo;
	pthread_mutex_lock(&(philo->info->fork)[philo->id]);
	print_status(TAKEN_A_FORK, philo);
	pthread_mutex_lock(&(philo->info->fork)[(philo->id + 1) % n]);
	print_status(TAKEN_A_FORK, philo);
	print_status(EATINGN, philo);
	update_last_ate_time(philo);
	high_accuracy_usleep_in_ms(philo->config->time_to_eat, philo);
	philo->number_of_meals++;
	pthread_mutex_unlock(&(philo->info->fork)[philo->id]);
	pthread_mutex_unlock(&(philo->info->fork)[(philo->id + 1) % n]);
}

static void	sleeping(t_philo *philo)
{
	print_status(SLEEPING, philo);
	high_accuracy_usleep_in_ms(philo->config->time_to_sleep, philo);
}

static void	thinking(t_philo *philo)
{
	int	sleep_time;
	int	n;

	n = 0;
	sleep_time = 0;
	if (philo->config->num_of_philo % 2 != 0
		&& philo->config->time_to_eat * 3 > philo->config->time_to_sleep)
	{
		n = philo->config->time_to_eat + philo->config->time_to_sleep;
		sleep_time = philo->config->time_to_eat * 3 - n;
	}
	print_status(THINKING, philo);
	if (sleep_time > 0)
		high_accuracy_usleep_in_ms(sleep_time, philo);
}

static void	waiting_to_die_with_a_fork(t_philo *philo)
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
	int		n;

	philo = (t_philo *)arg;
	n = philo->config->num_of_philo;
	if (philo->config->num_of_philo == 1)
		return (waiting_to_die_with_a_fork(philo));
	if ((philo->id + 1) % 2 == 0)
		usleep(500);
	if (n % 2 != 0 && (philo->id + 1) % n == 0)
		high_accuracy_usleep_in_ms(philo->config->time_to_eat * 2, philo);
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
