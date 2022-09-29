/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:25 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:27 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_died(int i, t_philo *philo)
{
	long	now_time;

	pthread_mutex_lock(philo[i].last_ate);
	now_time = get_elapsed_time_in_ms(philo->config->start_time);
	if (now_time - philo[i].last_ate_time >= philo->config->time_to_die)
	{
		pthread_mutex_unlock(philo[i].last_ate);
		pthread_mutex_lock(philo->info->end_flag);
		philo->info->end_simulation = 1;
		pthread_mutex_unlock(philo->info->end_flag);
		print_status(DIE, &philo[i]);
		return (1);
	}
	pthread_mutex_unlock(philo[i].last_ate);
	return (0);
}

void	big_brother(void *arg)
{
	int		i;
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(philo->info->end_meals);
		if (philo->info->n_of_finish_eat == philo->config->num_of_philo)
		{
			pthread_mutex_unlock(philo->info->end_meals);
			return ;
		}
		pthread_mutex_unlock(philo->info->end_meals);
		i = 0;
		while (i < philo->config->num_of_philo)
		{
			if (check_died(i, philo))
				return ;
			i++;
		}
		usleep(300);
	}
}
