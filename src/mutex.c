#include "philosophers.h"

void	create_fork(t_data *data)
{
	int		i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosopher);
	if (!data->forks)
		return ;
	while (i < data->number_of_philosopher)
	{
		pthread_mutex_init(&data ->forks[i], NULL);
		i++;
	}
}
/*initialisations des fourchettes(mutex) et retour de celle ci*/
// void destroy_all(t_data *data)
// {}

void	create_philo(t_data *data)
{
	int		i;

	i = 0;
	data->one_dead = 0;
	data-> philos = malloc
		(data-> number_of_philosopher * sizeof(t_philosopher));
	if (!data-> philos)
		return ;
	pthread_mutex_init(&data->death_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	data->start_time = get_time_in_ms();
	while (i < data->number_of_philosopher)
	{
		data->philos[i].last_meal_time = data->start_time;
		i++;
	}
	create_philo_plus(data, 0);
}

void	create_philo_plus(t_data *data, int i)
{
	while (data -> number_of_philosopher > i)
	{
		data->philos[i].id = i + 1;
		data-> philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks
		[(i + 1) % data->number_of_philosopher];
		pthread_mutex_init(&data->philos[i].meals_mutex, NULL);
		i++;
	}
}
/*creation de chaque philosophe avec les elements de sa structure*/

// void destroy_all(t_data *data)
// {
// 	pthread_mutex_destroy(&data->death_mutex);
// 	pthread_mutex_destroy(data ->forks);
// 	pthread_mutex_destroy(data->philos->right_fork);
// 	free(data->philos);
// 	free(data);
// 	EXIT_SUCCESS;
// }