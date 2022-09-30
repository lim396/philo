/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:08 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:10 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	malloc_error_handler(int n, t_overall *info, t_philo **philo)
{
	int	i;

	i = 0;
	if (philo != NULL)
	{
		while (i < n)
		{
			free((*philo)[i].last_ate);
			i++;
		}
		free(*philo);
	}
	free(info->fork);
	free(info->end_meals);
	free(info->get_time_and_print);
	free(info->end_flag);
	ft_putstr_fd("\e[1;31mmalloc error\e[0m\n", 2);
	return (1);
}

int	thread_error_handler(t_philo *philo, int flag)
{
	pthread_mutex_lock(philo->info->end_flag);
	philo->info->end_simulation = 1;
	pthread_mutex_unlock(philo->info->end_flag);
	return (flag);
}

void	print_args_error(void)
{
	ft_putstr_fd("\e[1;31margs error\e[0m\n", 2);
	ft_putstr_fd("\e[1;33mUsage\e[0m\n", 2);
	ft_putstr_fd("./philo number_of_philosophers time_to_die ", 2);
	ft_putstr_fd("time_to_eat time_to_sleep ", 2);
	ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n\n", 2);
	ft_putstr_fd("Absolutely necessary except for ", 2);
	ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n", 2);
	ft_putstr_fd("Arguments are valid only for numbers in the range from ", 2);
	ft_putstr_fd("1 to the INT_MAX(2147483647)\n", 2);
}
