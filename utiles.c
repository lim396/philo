#include "philo.h"

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	return ;
}

int	ft_atoi(const char *str)
{
	long	integer;
	int		sign;

	integer = 0;
	sign = 1;
	while (('\t' <= *str && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		sign = (44 - *str);
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		if (sign * integer > (LONG_MAX - (*str - '0')) / 10)
			return ((int)LONG_MAX);
		else if (sign * integer < (LONG_MIN + (*str - '0')) / 10)
			return ((int)LONG_MIN);
		integer = (integer * 10) + (*str - '0');
		str++;
	}
	return (integer * sign);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*reserve_mem;

	if (!(count && size))
	{
		count = 1;
		size = 1;
	}
	if (count && (size > SIZE_MAX / count))
		return (NULL);
	reserve_mem = (void *)malloc(count * size);
	if (!reserve_mem)
		return (NULL);
	memset(reserve_mem, '\0', count * size);
	return (reserve_mem);
}
