/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_thread.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:48 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:50 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	thread_processing(int n, t_philo *philo)
{
	int			i;
	int			j;
	pthread_t	monitor_thread;
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
	if (pthread_create(&monitor_thread, NULL, (void *)monitor, philo))
		error_flag |= thread_error_handler(philo, MONITOR_ERROR);
	j = 0;
	while (j < i)
		pthread_join(philo[j++].thread, NULL);
	if (!(error_flag & MONITOR_ERROR))
		pthread_join(monitor_thread, NULL);
	if (error_flag > 0)
		return (1);
	return (0);
}
