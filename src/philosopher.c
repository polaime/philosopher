#include "philosophers.h"

void	*philosopher_life(void *info)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)info;
	while (!is_simulation_over(philo))
	{
		print_log(philo, "is thinking");
		pthread_mutex_lock(philo->left_fork);
		if (is_simulation_over(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		print_log(philo, "took the left fork");
		pthread_mutex_lock(philo->right_fork);
		if (is_simulation_over(philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		print_log(philo, "took the right fork");
		philosopher_life_plus(philo);
	}
	return (NULL);
}

void	philosopher_life_plus(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meals_mutex);
	philo->last_meal_time = get_time_in_ms();
	print_log(philo, "is eating");
	usleep(philo->data->time_to_eat * 1000);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(&philo->meals_mutex);
		return ;
	}
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&philo->meals_mutex);
	print_log(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

void	*monitor(void *info)
{
	t_data			*data;
	int				i;
	long long int	now;

	data = (t_data *)info;
	while (1)
	{
		i = 0;
		while (i < data->number_of_philosopher)
		{
			pthread_mutex_lock(&data->philos[i].meals_mutex);
			now = get_time_in_ms();
			if (!routine_monitor(data, i, now))
				return (NULL);
			pthread_mutex_unlock(&data->philos[i].meals_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

int	is_simulation_over(t_philosopher *philo)
{
	int		died;

	if (philo -> meals_eaten >= philo -> data
		->number_of_times_each_philosopher_must_eat)
		return (1);
	pthread_mutex_lock(&philo->data->death_mutex);
	died = philo->data->one_dead;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (died);
}

int	routine_monitor(t_data *data, int i, long long int now)
{
	if (data->number_of_times_each_philosopher_must_eat > data
		-> philos ->meals_eaten)
	{
		pthread_mutex_unlock(&data->philos[i].meals_mutex);
		return (0);
	}
	if (now - data->philos[i].last_meal_time > data->time_to_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->one_dead = 1;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		print_log_dead(data->philos, "is dead");
		pthread_mutex_unlock(&data->print_mutex);
		pthread_mutex_unlock(&data->philos[i].meals_mutex);
		return (0);
	}
	return (1);
}
