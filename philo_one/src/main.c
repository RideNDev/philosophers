#include "../inc/philosophers.h"

t_data		*g_data;

int			main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (0);
	g_data = &data;
	init_game(ac, av);
	start_game();
	return (0);
}

//---------------------------- INIT_GAME -----------------------------------

int			init_game(int ac, char **av)
{
	g_data->nb = ft_atoi(av[1]);
	g_data->time_to_die = ft_atoi(av[2]);
	g_data->time_to_eat = ft_atoi(av[3]);
	g_data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		g_data->number_of_time_each_philosophers_must_eat = ft_atoi(av[5]);
	else
		g_data->number_of_time_each_philosophers_must_eat = 2147483647;
	if (!(g_data->philo = malloc(sizeof(t_philo) * g_data->nb)))
		return (0);
	if (!(g_data->fork = malloc(sizeof(pthread_mutex_t) * g_data->nb)))
		return (0);
	init_philosophers();
	return (1);
}

void		init_philosophers()
{
	int		i;

	i = 0;
	while (i < g_data->nb)
	{
		g_data->philo[i].left = &(g_data->fork[i]);
		g_data->philo[i].right = &(g_data->fork[(i + 1) % g_data->nb]);
		g_data->philo[i].last_eat = 0;
		g_data->philo[i].name = i + 1;
		i++;
	}
}

//----------------------------- START_GAME -----------------------------------

int			start_game()
{
	int			i;
	pthread_t	tid;

	i = 0;
	while (i < g_data->nb)
	{
		pthread_create(&tid, NULL, &ft_life, &g_data->philo[i]);
		pthread_detach(tid);
		usleep(50);
		i++;
	}
	return (0);
}

int			get_time(void)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	return (((int)(tmp.tv_sec) * 1000) + ((int)(tmp.tv_usec) / 1000));
}

void		message(t_philo *philo, int msg)
{
	int		time;

	time = get_time() - g_data->time_start;
	ft_putnbr_fd(time, 1);
	write(1, "\t", 1);
	ft_putnbr_fd(philo->name, 1);
	if (msg == 1)
		write(1, " is thinking\n", 13);
	if (msg == 2)
		write(1, " is eating\n", 11);
	if (msg == 3)
		write(1, " is sleeping\n", 13);
	if (msg == 4)
		write(1, " has taken a fork\n", 18);
}


//------------------------------ PHILO_LIFE ------------------------------------

void		*ft_life(void *tmp_philo)
{
	t_philo	*philo;

	philo = (t_philo *)tmp_philo;
	while (1)
	{
		//------- EAT ------------------
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(philo->right);
		message(philo, 4);
		message(philo, 4);
		philo->last_eat = get_time() - g_data->time_start;
		message(philo, 2);
		usleep(g_data->time_to_eat * 1000);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		//------ SLEEP -----------------
		message(philo, 3);
		usleep(g_data->time_to_sleep * 1000);	
		//------ THINK -----------------
		message(philo, 1);
	}
	return (NULL);
}

//-------------------------------------------------------------------------------



