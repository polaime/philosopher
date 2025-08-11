#include "philosophers.h"

void *philosopher_life(void *info)
{
    t_philosopher *philo = (t_philosopher *)info;

    while (1)
{
    if (is_simulation_over(philo->data)) break;
    printf("Philosopher %d is thinking\n", philo->id);

    if (is_simulation_over(philo->data)) break;
    pthread_mutex_lock(philo->left_fork);
    printf("Philosopher %d took the left fork\n", philo->id);

    if (is_simulation_over(philo->data))
    {
        pthread_mutex_unlock(philo->left_fork);
        break;
    }
    pthread_mutex_lock(philo->right_fork);
    printf("Philosopher %d took the right fork\n", philo->id);

    if (is_simulation_over(philo->data))
    {
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        break;
    }

    pthread_mutex_lock(&philo->meals_mutex);
    philo->last_meal_time = get_time_in_ms();
    printf("Philosopher %d is eating\n", philo->id);
    usleep(philo->data->time_to_eat * 1000);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meals_mutex);

    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);

    if (is_simulation_over(philo->data)) break;
    printf("Philosopher %d is sleeping\n", philo->id);
    usleep(philo->data->time_to_sleep * 1000);
}

    return NULL;
}
void *monitor(void *info)
{
	t_data *data;
	int i;
	long long int now;

	data = (t_data *)info;
	while (1)
	{
		i = 0;
		while (i < data->number_of_philosopher)
		{
			pthread_mutex_lock(&data->philos[i].meals_mutex);
			now = get_time_in_ms();
			printf("[monitor] Philosopher %d | now = %lld | last_meal_time = %lld | diff = %lld | time_to_die = %d\n",
       			data->philos[i].id, now, data->philos[i].last_meal_time, now - data->philos[i].last_meal_time, data->time_to_die);
			if (now - data->philos[i].last_meal_time > data->time_to_die)
			{
				pthread_mutex_lock(&data->death_mutex);
				data->one_dead = 1;
				pthread_mutex_unlock(&data->death_mutex);
				pthread_mutex_lock(&data->print_mutex);
				printf("Philosopher %d died\n", data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				pthread_mutex_unlock(&data->philos[i].meals_mutex);
				return NULL;
			}
			pthread_mutex_unlock(&data->philos[i].meals_mutex);
			i++;
		}
		usleep(1000);
	}
	return NULL;
}

int is_simulation_over(t_data *data)
{
	int res;

	pthread_mutex_lock(&data->death_mutex);
	res = data->one_dead;
	pthread_mutex_unlock(&data->death_mutex);
	return (res);
}
