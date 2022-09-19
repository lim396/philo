#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>

#define TAKEN_A_FORK 0
#define EATINGN 1
#define SLEEPING 2
#define THINKING 3
#define DIE 4
//mutex fork * philonum
//mutex eat_count

//init mutex two pattern
//pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_init(&mutex, NULL);

typedef struct s_config	t_config;
struct s_config
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat; //too long must short
	long	start_time;
//	pthread_mutex_t	*fork;
//	pthread_mutex_t	*finish_must_eat;
//	int	finish_must_eat;
//	pthread_mutex_t	*get_time_and_print;
};

typedef struct s_overall_info	t_overall_info;
struct s_overall_info
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	*end_meals;
	int	n_of_finish_eat;
	pthread_mutex_t	*get_time_and_print;
	pthread_mutex_t	*end_flag;
	bool	end_simulation;
};

typedef struct s_philo	t_philo;
struct s_philo
{
	int id; // while i
	int	number_of_meals;
	long	last_ate_time;
	pthread_mutex_t	*last_ate;
//	pthread_mutex_t	*left_fork;
//	pthread_mutex_t	*right_fork;
//	pthread_mutex_t	*fork;
//	int forkleft; //0
//	int forkright; //0
	pthread_t thread; //each thread
	t_overall_info	*info;
	t_config		*config;
//	struct Philosophyer *prev;
//	struct Philosophyer *next;
};

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

bool	is_natural_num_ex_zero(const char *str)
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

void	init_config(char **argv, t_config *config)
{
	config->number_of_philosophers = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	config->number_of_times_each_philosopher_must_eat = -1;
	if (argv[5])
		config->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
}

void	init_overall_info(int n, t_overall_info *info)
{
	int	i;

	info->n_of_finish_eat = 0;
	info->end_simulation = 0;
	info->end_meals = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(info->end_meals, NULL);
	info->get_time_and_print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(info->get_time_and_print, NULL);
	info->end_flag = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(info->end_flag, NULL);
	info->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	i = 0;
	while (i < n)
	{
		pthread_mutex_init(&(info->fork)[i], NULL);
		i++;
	}
}

void	init_philosophers(t_config *config, t_overall_info *info, t_philo **philo)
{
	int	i;
	int	n;

	n = config->number_of_philosophers;
	*philo = (t_philo *)malloc(sizeof(t_philo) * n);
	i = 0;
	while (i < n)
	{
		(*philo)[i].id = i;
		(*philo)[i].number_of_meals = 0;
		(*philo)[i].config = config;
		(*philo)[i].info = info;
		(*philo)[i].last_ate_time = 0;
		(*philo)[i].last_ate = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((*philo)[i].last_ate, NULL);
		i++;
	}
}

bool	check_end(t_philo *philo)
{
//	printf("pre lock\n");
	pthread_mutex_lock(philo->info->end_flag);
	if (philo->info->end_simulation)
	{
		pthread_mutex_unlock(philo->info->end_flag);
		return (true);
	}
	pthread_mutex_unlock(philo->info->end_flag);
	return (false);
}

//timestamp_in_ms X has taken a fork
//timestamp_in_ms X is eating
//timestamp_in_ms X is sleeping
//timestamp_in_ms X is thinking
//timestamp_in_ms X died
long	get_elapsed_time_in_ms(long start_time)
{
	struct timeval	now_time;
	long			elapsed_time;

	gettimeofday(&now_time, NULL);
	elapsed_time = (now_time.tv_sec * 1e3 + now_time.tv_usec * 1e-3) - start_time;
//	printf("elapsed %ld\n", elapsed_time);
	return (elapsed_time);
}


void	print_status(int status, t_philo *philo)
{
//	struct timeval	now_time;
	long	just_now;

//	if (check_end(philo))
//		return ;
//	just_now = get_elapsed_time_in_ms(philo->config->start_time);
//	if (status == DIE)
//		printf("%ld Philosopher %d died\n", just_now, philo->id);
	pthread_mutex_lock(philo->info->get_time_and_print);
//	gettimeofday(&now_time, NULL);
//	printf("time %ld\n", philo->config->start_time);
//	just_now = (now_time.tv_sec * 1e3 + now_time.tv_usec * 1e-3) - philo->config->start_time;

	just_now = get_elapsed_time_in_ms(philo->config->start_time);
	if (status == TAKEN_A_FORK && !check_end(philo))
		printf("%ld Philosopher %d has taken a fork\n", just_now, philo->id + 1);
	else if (status == EATINGN && !check_end(philo))
		printf("%ld Philosopher %d is eating\n", just_now, philo->id + 1);
	else if (status == SLEEPING && !check_end(philo))
		printf("%ld Philosopher %d is sleeping\n", just_now, philo->id + 1);
	else if (status == THINKING && !check_end(philo))
		printf("%ld Philosopher %d is thinking\n", just_now, philo->id + 1);
	else if (status == DIE)
		printf("%ld Philosopher %d died\n", just_now, philo->id + 1);
	pthread_mutex_unlock(philo->info->get_time_and_print);
}

void	update_last_ate_time(t_philo *philo)
{
//	struct timeval now;
	
	pthread_mutex_lock(philo->last_ate);
//	gettimeofday(&now, NULL);
//	printf("time %ld\n", philo->config->start_time);
//	philo->last_ate_time = (now.tv_sec * 1e3 + now.tv_usec * 1e-3) - philo->config->start_time;
	philo->last_ate_time = get_elapsed_time_in_ms(philo->config->start_time);
//	printf("id: %d ate_time: %ld\n", philo->id, philo->last_ate_time);
	pthread_mutex_unlock(philo->last_ate);
}

void	eating(t_philo *philo)
{
	long	sleep_start_time;

//	if (check_end(philo))
//		return ;
	print_status(TAKEN_A_FORK, philo);
	print_status(TAKEN_A_FORK, philo);
	print_status(EATINGN, philo);
	update_last_ate_time(philo);
	sleep_start_time = get_elapsed_time_in_ms(philo->config->start_time);
	while (get_elapsed_time_in_ms(philo->config->start_time) - sleep_start_time < philo->config->time_to_eat)
		usleep(10);
//	update_last_ate_time(philo);
//	printf("id: %d last_time %ld\n", philo->id, philo->last_ate_time);
//	usleep(philo->config->time_to_eat * 1e3);
	philo->number_of_meals++;
}

void	sleeping(t_philo *philo)
{
	long	sleep_start_time;

//	if (check_end(philo))
//		return ;
	print_status(SLEEPING, philo);
	sleep_start_time = get_elapsed_time_in_ms(philo->config->start_time);
	while (get_elapsed_time_in_ms(philo->config->start_time) - sleep_start_time < philo->config->time_to_eat)
		usleep(10);
//	usleep(philo->config->time_to_sleep * 1e3);
}

void	thinking(t_philo *philo)
{
//	if (check_end(philo))
//		return ;
	print_status(THINKING, philo);
//	usleep(200); //necessary?
}

int	ft_abs(int num)
{
	if (num < 0)
		return (-(num + 1) + 1u);
	return (num);
}

void	routine(void *arg)
{
	int	i;
	int	n;
	int	ones_digit;
//	int	sleep_start_time;
	t_philo *philo;

	philo = (t_philo *)arg;
//	printf("num_of_philo %d\n", philo->config->number_of_philosophers);
//	printf("thread ok\n");
	n = philo->config->number_of_philosophers;
//	if (((philo->id + 1) % 2 == 0 && (philo->id + 1) % 4 != 0) || ((philo->id + 1) % 5 == 0))
	if (((philo->id + 1) % 2 == 0) || ((philo->id + 1) % 5 == 0))
		usleep(500);
//		usleep(philo->config->time_to_eat * 1000);
//	if ((philo->id + 1) % 4 == 0)
//		usleep(200);
//		usleep(philo->config->time_to_eat * 1000 / 2);
//	ones_digit = (philo->id + 1) % 10;
//	if (ones_digit > 5 || ones_digit == 0)
//		ones_digit = ft_abs(ones_digit - 5);
//	if ((philo->id + 1) % 3 == 0)
//	{
//		while (get_elapsed_time_in_ms(philo->config->start_time) - philo->last_ate_time + 10 < philo->config->time_to_die)
//		usleep(10);
//		sleep_start_time = get_elapsed_time_in_ms(philo->config->start_time);
//		while (get_elapsed_time_in_ms(philo->config->start_time) - sleep_start_time < (philo->config->time_to_eat))
//		usleep(10);
//	}
//	i = 0;
//	if (ones_digit == 4 || ones_digit == 5)
//		i = 1;
//	printf("conf ok\n");
	while (!check_end(philo))
	{
//		ones_digit = (philo->id + 1) % 10;
//		if (ones_digit > 5 || ones_digit == 0)
//			ones_digit = ft_abs(ones_digit - 5);
//		if (i == 3)
//			i = 0;
//		if (i == 2)
//		{
////			sleep_start_time = get_elapsed_time_in_ms(philo->config->start_time);
//			while (get_elapsed_time_in_ms(philo->config->start_time) - philo->last_ate_time < philo->config->time_to_die / 2)
//			usleep(10);
//		}
//			i = 0;
		pthread_mutex_lock(&(philo->info->fork)[philo->id]);
		pthread_mutex_lock(&(philo->info->fork)[(philo->id + 1) % n]);
//		printf("mutex ok\n");
		eating(philo);
//		printf("eat ok\n");
		pthread_mutex_unlock(&(philo->info->fork)[philo->id]);
		pthread_mutex_unlock(&(philo->info->fork)[(philo->id + 1) % n]);
		if (philo->number_of_meals == philo->config->number_of_times_each_philosopher_must_eat)
		{
			pthread_mutex_lock(philo->info->end_meals);
			philo->info->n_of_finish_eat++;
			pthread_mutex_unlock(philo->info->end_meals);
			return ;
		}
		sleeping(philo);
		thinking(philo);
		i++;
	}
	return ;
}

void	big_brother(void *arg)
{
	int	i;
	t_philo	*philo;
	long	now_time;
//	struct timeval	now;

	philo = (t_philo *)arg;
	while (1)
	{
		
		pthread_mutex_lock(philo->info->end_meals);
		if (philo->info->n_of_finish_eat == philo->config->number_of_philosophers)
		{
			pthread_mutex_unlock(philo->info->end_meals);
			return ;
		}
		pthread_mutex_unlock(philo->info->end_meals);
		i = 0;
		while (i < philo->config->number_of_philosophers)
		{
			pthread_mutex_lock(philo[i].last_ate);
//			gettimeofday(&now, NULL);
			//printf("time %ld\n", philo->config->start_time);
//			now_time = (now.tv_sec * 1e3 + now.tv_usec * 1e-3) - philo->config->start_time;
			now_time = get_elapsed_time_in_ms(philo->config->start_time);
			if (now_time - philo[i].last_ate_time >= philo->config->time_to_die)
			{
//				printf("id: %d last_ate: %ld\n", i, philo[i].last_ate_time);
				pthread_mutex_unlock(philo[i].last_ate);
				pthread_mutex_lock(philo->info->end_flag);
//				printf("die_time: %ld\n", get_elapsed_time_in_ms(philo->config->start_time));
				philo->info->end_simulation = 1;
				pthread_mutex_unlock(philo->info->end_flag);
				print_status(DIE, &philo[i]);
				return ;
			}
			pthread_mutex_unlock(philo[i].last_ate);
			i++;
		}
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_overall_info	info;
	t_philo		*philo;
	pthread_t	monitor;
	int	i;
	int	debg;
	struct timeval	now;

	if (!check_args(argc, argv))
		return (1);
	init_config(argv, &config); //include init_forks?
//	printf("ok\n");
	init_overall_info(config.number_of_philosophers, &info);
//	printf("ok\n");
		//init_forks(config.number_of_philosophers, &config);
	init_philosophers(&config, &info, &philo);
//	printf("ok\n");
	gettimeofday(&now, NULL);
	philo->config->start_time = (now.tv_sec * 1e3) + (now.tv_usec * 1e-3);
	i = 0;
//	printf("num_of_philo %d\n", config.number_of_philosophers);
	while (i < config.number_of_philosophers)
	{
		debg = pthread_create(&philo[i].thread, NULL, (void *)routine, &philo[i]);
//		printf("creat %d\n", i);
		if (debg != 0)
			printf("creat ko\n");
		i++;
	}
	pthread_create(&monitor, NULL, (void *)big_brother, philo);
	i = 0;
	while (i < config.number_of_philosophers)
	{
		debg = pthread_join(philo[i].thread, NULL);
		if (debg != 0)
			printf("join ko\n");
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < config.number_of_philosophers)
	{
		pthread_mutex_destroy(philo[i].last_ate);
		free(philo[i].last_ate);
		pthread_mutex_destroy(&(info.fork)[i]);
		i++;
	}
	free(info.fork);
	pthread_mutex_destroy(info.end_meals);
	pthread_mutex_destroy(info.get_time_and_print);
	pthread_mutex_destroy(info.end_flag);
	free(info.end_meals);
	free(info.get_time_and_print);
	free(info.end_flag);
	free(philo);
	

		//check_args
		//init
		//	struct? varible = atoi argv[1~5] && isdigit
		//	init_forks
		//	init_mutex
		//	init_philo_info
		//while(i < philo_num)
		//	creat thread
		//
		//routine
		//	lock r_fork
		//	lock l_fork
		//	eat print       mutex_lock
		//	unlock r_fork
		//	unlock l_fork
		//	sleep print		get now_time and print
		//	think print		mutex_unlock
		//
		//big brother
		//	while (number_of_times_each_philosopher_must_eat != true)
		//		if (now - last_ate_time >= time_to_die)
		//			philo.die/config.die = true;
		//			mutex_lock
		//			get now_time and print die
		//			mutex_unlock
		//
		//i = 0;
		//while(i < philo_num)
		//	crrat join
		//return ;
	return (0);
}
