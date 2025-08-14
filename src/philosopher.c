#include "philosophers.h"

void	*philosopher_life(void *info)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)info;
	while (!is_simulation_over(philo))
	{
		printf("Philosopher %d is thinking\n", philo->id);
		pthread_mutex_lock(philo->left_fork);
		if (is_simulation_over(philo))
		{
			pthread_mutex_unlock(philo-> left_fork);
			break ;
		}
		printf("Philosopher %d took the left fork\n", philo->id);
		pthread_mutex_lock(philo->right_fork);
		if (is_simulation_over(philo))
		{
			pthread_mutex_unlock(philo-> right_fork);
			pthread_mutex_unlock(philo-> left_fork);
			break ;
		}
		printf("Philosopher %d took the right fork\n", philo->id);
		pthread_mutex_lock(&philo->meals_mutex);
		if (is_simulation_over(philo))
		{
			pthread_mutex_unlock(philo-> right_fork);
			pthread_mutex_unlock(philo-> left_fork);
			pthread_mutex_unlock(&philo->meals_mutex);
			break ;
		}
		philosopher_life_plus(philo);
	}
	return (NULL);
}

void	philosopher_life_plus(t_philosopher *philo)
{
	philo->last_meal_time = get_time_in_ms();
	printf("Philosopher %d is eating\n", philo->id);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(philo-> right_fork);
		pthread_mutex_unlock(philo-> left_fork);
		pthread_mutex_unlock(&philo->meals_mutex);
		return ;
	}
	usleep(philo->data->time_to_eat * 1000);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(philo-> right_fork);
		pthread_mutex_unlock(philo-> left_fork);
		pthread_mutex_unlock(&philo->meals_mutex);
		return ;
	}
	philo->meals_eaten++;
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(philo-> right_fork);
		pthread_mutex_unlock(philo-> left_fork);
		pthread_mutex_unlock(&philo->meals_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->right_fork);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(philo-> left_fork);
		pthread_mutex_unlock(&philo->meals_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->left_fork);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(&philo->meals_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->meals_mutex);
	is_simulation_over(philo);
	printf("Philosopher %d is sleeping\n", philo->id);
	is_simulation_over(philo);
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
			if (now - data->philos[i].last_meal_time > data->time_to_die)
			{
				pthread_mutex_lock(&data->death_mutex);
				data->one_dead = 1;
				printf(">>> DEBUG: one_dead mis à 1\n");
				pthread_mutex_unlock(&data->death_mutex);
				pthread_mutex_lock(&data->print_mutex);
				printf("Philosopher %d died\n", data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				pthread_mutex_unlock(&data->philos[i].meals_mutex);
				pthread_exit(NULL);
				return (NULL);
			}
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

	pthread_mutex_lock(&philo->data->death_mutex);
	died = philo->data->one_dead;
	pthread_mutex_unlock(&philo->data->death_mutex);
	if (died)
		pthread_exit(NULL);
	return (died);
}
