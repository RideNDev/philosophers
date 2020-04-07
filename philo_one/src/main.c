#include "../inc/philosophers.h"

t_data			*g_data;

int				main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (0);
	g_data = &data;
	if (!(init_game(ac, av)))
		return (0);
	if (!(start_game()))
		return (0);
	while (1)
	{
		if (end_game())
		{
			clean();
			return (0);
		}
	}
	return (0);
}

//---------------------------- INIT_GAME -----------------------------------

int				init_game(int ac, char **av)
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
	pthread_mutex_init(&g_data->msg, NULL);
	g_data->philo_dead = 0;
	g_data->philo_ok = 0;
	init_philosophers();
	return (1);
}

void			init_philosophers()
{
	int		i;

	i = 0;
	while (i < g_data->nb)
	{
		pthread_mutex_init(&g_data->fork[i], NULL);
		g_data->philo[i].left = &(g_data->fork[i]);
		g_data->philo[i].right = &(g_data->fork[(i + 1) % g_data->nb]);
		g_data->philo[i].last_eat = 0;
		g_data->philo[i].name = i + 1;
		g_data->philo[i].nb_of_eat = 0;
		i++;
	}
}

void	clean()
{
	int i;

	i = 0;
	while (i < g_data->nb)
	{
		pthread_mutex_destroy(&g_data->fork[i]);
//		pthread_mutex_destroy(&g_data->philo[i].is_eating);
		i++;
	}
	pthread_mutex_destroy(&g_data->msg);
	free(g_data->fork);
	free(g_data->philo);
}

//----------------------------- START_GAME -----------------------------------

int				start_game()
{
	int			i;
	pthread_t	tid;

	i = 0;
	g_data->time_start = get_time();
	while (i < g_data->nb)
	{
		if (pthread_create(&tid, NULL, &ft_philo, &g_data->philo[i]))
			return (0);
		g_data->philo[i].last_eat = g_data->time_start;
//---> init dans philo2 et 3
		i++;
	}
	return (1);
}

int				end_game()
{
	int		time;

	if (g_data->philo_dead)
	{
		pthread_mutex_lock(&g_data->msg);
		time = get_time() - g_data->time_start;
		ft_putnbr_fd(time, 1);
		write(1, "\t", 1);
		ft_putnbr_fd(g_data->philo_dead, 1);
		write(1, " died\n", 6);
		return (1);
	}
	if (g_data->philo_ok >= g_data->nb)
	{
		pthread_mutex_lock(&g_data->msg);
		write(1, "Everybody has eaten enough times.\n", 34);
		return (1);
	}
	return (0);
}

int				get_time(void)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	return ((tmp.tv_sec * (uint64_t)1000) + (tmp.tv_usec / 1000));
}

void			message(t_philo *philo, int msg)
{
	int		time;

	pthread_mutex_lock(&g_data->msg);
	time = get_time() - g_data->time_start;
	ft_putnbr_fd(time, 1);
	write(1, "\t", 1);
	ft_putnbr_fd(philo->name, 1);
	if (msg == 1)
		write(1, " is thinking\n", 13);
	else if (msg == 2)
		write(1, " is eating\n", 11);
	else if (msg == 3)
		write(1, " is sleeping\n", 13);
	else if (msg == 4)
		write(1, " has taken a fork\n", 18);
	pthread_mutex_unlock(&g_data->msg);
}


//------------------------------ PHILO_LIFE ------------------------------------

void			*ft_philo(void *tmp_philo)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo *)tmp_philo;
	if (pthread_create(&tid, NULL, &check_life, philo))
		return (void*)1;
	while (1)
	{
		//------ THINK -----------------
		message(philo, 1);
		//------- EAT ------------------
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(philo->right);
		message(philo, 4);
		message(philo, 4);
		philo->last_eat = get_time();
		(philo->nb_of_eat)++;
		message(philo, 2);
		usleep(g_data->time_to_eat * 1000);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		if (philo->nb_of_eat >= g_data->number_of_time_each_philosophers_must_eat)
		{	
			g_data->philo_ok++;
			break;
		}
		//------ SLEEP -----------------
		message(philo, 3);
		usleep(g_data->time_to_sleep * 1000);	
	}
	return (NULL);
}

void			*check_life(void *tmp_philo)
{
	t_philo *philo;

	philo = (t_philo *)tmp_philo;
	while (1)
	{
		if (get_time() - philo->last_eat >= g_data->time_to_die)
			g_data->philo_dead = philo->name;
	}
	return (NULL);
}

//-------------------------------------------------------------------------------

