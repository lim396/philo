/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:46:19 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:46:21 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_overall		info;
	t_philo			*philo;
	struct timeval	now;

	if (!check_args(argc, argv))
		return (1);
	init_config(argv, &config);
	if (init_overall_info(config.num_of_philo, &info) != 0)
		return (1);
	if (init_philosophers(&config, &info, &philo) != 0)
		return (1);
	init_mutex(config.num_of_philo, &info, &philo);
	gettimeofday(&now, NULL);
	philo->config->start_time = (now.tv_sec * 1e3) + (now.tv_usec * 1e-3);
	if (thread_processing(config.num_of_philo, philo) != 0)
	{
		ft_putstr_fd("\e[1;31mpthread_create error\e[0m\n", 2);
		return (1);
	}
	destroy_mutex(config.num_of_philo, &info, &philo);
	return (0);
}
