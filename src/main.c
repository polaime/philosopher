#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philos time_to_die time_to_eat time_to_sleep [number_of_times_each_philo_must_eat]\n");
		return (1);
	}
	data.number_of_philosopher = ft_atoi(argv[1]);
	data.time_to_sleep = ft_atoi(argv[3]);
	data.time_to_eat = ft_atoi(argv[4]);
	data.time_to_die = ft_atoi(argv[2]);
	if (argc == 6)
		data.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		data.number_of_times_each_philosopher_must_eat = -1;
	create_fork(&data);
	create_philo(&data);
	one_philo_exist(&data);
	run_simu(&data);
	destroy_all(&data);
}
/*creation et envoie a ma structure des differentes 
valeurs de chaque philosophe*/

void	run_simu(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	start_simulation(data);
	if (pthread_create(&monitor_thread, NULL, monitor, data) != 0)
		return ;
	pthread_join(monitor_thread, NULL);
	while (i < data->number_of_philosopher)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data-> number_of_philosopher)
	{
		if (pthread_create(&data-> philos[i].thread, NULL, philosopher_life,
				&data->philos[i]))
		{
			printf("failed to create thread for philosopher nmbr: %d\n", i + 1);
		}
		i++;
	}
}
void one_philo_exist(t_data *data)
{
	if (data->number_of_philosopher == 1)
	{
		print_log(&data->philos[0], "has taken a fork");
		usleep(data->time_to_die * 1000);
		pthread_mutex_lock(&data->death_mutex);
		data->one_dead = 1;
		pthread_mutex_unlock(&data->death_mutex);
		print_log(&data->philos[0], "is dead");
		return ;
	}
}