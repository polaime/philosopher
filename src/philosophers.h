
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/time.h>
# include <string.h>
# include <pthread.h>

typedef struct s_philosopher {
	int					id;
	int					meals_eaten;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	long long			last_meal_time;
	pthread_mutex_t		meals_mutex;
	struct s_data		*data;
	pthread_t			thread;
}	t_philosopher;

typedef struct s_data {
	int				number_of_times_each_philosopher_must_eat;
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				one_dead;
	int				meals_required;
	t_philosopher	*philos;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}	t_data;


//mutex.c
void	create_fork(t_data *data);
void	create_philo(t_data *data);
void	destroy_all(t_data *data);
void	create_philo_plus(t_data *data, int i);

//main.c
int		main(int argc, char **argv);
void	start_simulation(t_data *data);
void	run_simu(t_data *data);

//utils.c
long long	get_time_in_ms(void);
int		ft_atoi(const char *str);
//philosopher.c
void	*philosopher_life(void *info);
void	philosopher_life_plus(t_philosopher *philo);
void	*monitor(void *info);
int		is_simulation_over(t_philosopher *philo);

#endif