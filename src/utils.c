#include "philosophers.h" 

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

int	ft_atoi(const char *str)
{
	int	i;
	int	nb;
	int	r;

	nb = 0;
	i = 0;
	r = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == ' ' )
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			r++;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (nb * 10);
		nb = nb + str[i] - '0';
		i++;
	}
	if (r % 2 == 1)
		nb = -nb;
	return (nb);
}

void	print_log(t_philosopher *philo, char *message)
{
	long long	timestamp;

	if (is_simulation_over(philo))
		return ;
	timestamp = get_time_in_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_over(philo))
		printf("%lld Philosopher %d %s\n", timestamp, philo->id, message);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
