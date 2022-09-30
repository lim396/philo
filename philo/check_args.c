/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 02:45:43 by shongou           #+#    #+#             */
/*   Updated: 2022/09/30 02:45:45 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_natural_num_ex_zero(const char *str)
{
	long long	decimal;

	decimal = 0;
	if (*str == '+')
		str++;
	if (*str == '-')
		return (false);
	if (*str == '0' || *str == '\0')
		return (false);
	while ('0' <= *str && *str <= '9')
	{
		if (decimal > (INT_MAX - (*str - '0')) / 10)
			return (false);
		decimal = (decimal * 10) + (*str - '0');
		str++;
	}
	return (true);
}

bool	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc == 5 || argc == 6)
	{
		i = 1;
		while (i < argc)
		{
			j = 0;
			while (argv[i][j])
			{
				if (!(ft_isdigit(argv[i][j])))
					return (false);
				j++;
			}
			if (!(is_natural_num_ex_zero(argv[i])))
				return (false);
			i++;
		}
		return (true);
	}
	return (false);
}
