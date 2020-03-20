#include "../inc/philosophers.h"

t_data			*g_data;

int				main(int ac, char **av)
{
	t_data	data;

	if (ac > 6 || ac < 5)
		return (0);
	sem_unlink(FORK);
	sem_unlink(MSG);
	g_data = &data;
	init_game(ac, av);
	start_game();
	while (1)
	{
		if (end_game())
		{
			sem_unlink(FORK);
			sem_unlink(MSG);
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
	g_data->fork = sem_open(FORK, O_CREAT | O_EXCL, 0644, g_data->nb);
	g_data->msg = sem_open(MSG, O_CREAT | O_EXCL, 0644, 1);
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
		g_data->philo[i].last_eat = 0;
		g_data->philo[i].name = i + 1;
		g_data->philo[i].nb_of_eat = 0;
		i++;
	}
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
		g_data->philo[i].pid = fork();
		if (g_data->philo[i].pid < 0)
			return (1);
		else if (g_data->philo[i].pid == 0)
		{
			ft_philo(&g_data->philo[i]);
			exit(1);
		}
	}
	return (0);
}
//---------------------------------------------> gestion ded ??
int				end_game()
{
	int		time;

	if (g_data->philo_dead)
	{
		sem_wait(g_data->msg);
		time = get_time() - g_data->time_start;
		ft_putnbr_fd(time, 1);
		write(1, "\t", 1);
		ft_putnbr_fd(g_data->philo_dead, 1);
		write(1, " died\n", 6);
		sem_post(g_data->msg);
		return (1);
	}
//	if (g_data->philo_ok >= g_data->nb)
//	{
//		sem_wait(g_data->msg);
//		write(1, "Everybody has eaten enough times.\n", 34);
//		sem_post(g_data->msg);
//		return (1);
//	}
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

	sem_wait(g_data->msg);
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
	sem_post(g_data->msg);
}


//------------------------------ PHILO_LIFE ------------------------------------

void			ft_philo(t_philo *philo)
{
	pthread_t	tid;

	pthread_create(&tid, NULL, &check_life, philo);
	while (1)
	{
		//------ THINK -----------------
		message(philo, 1);
		//------- EAT ------------------
//		if (philo->nb_of_eat >= g_data->number_of_time_each_philosophers_must_eat)
//		{	
//			g_data->philo_ok++;
//			break;
//		}
		sem_wait(g_data->fork);
		sem_wait(g_data->fork);
		message(philo, 4);
		message(philo, 4);
		philo->last_eat = get_time();
//		(philo->nb_of_eat)++;
		message(philo, 2);
		usleep(g_data->time_to_eat * 1000);
		sem_post(g_data->fork);
		sem_post(g_data->fork);
		//------ SLEEP -----------------
		message(philo, 3);
		usleep(g_data->time_to_sleep * 1000);	
	}
}

void			*check_life(void *tmp_philo)
{
	t_philo *philo;

	philo = (t_philo *)tmp_philo;
	while (1)
	{
		if (get_time() - philo->last_eat >= g_data->time_to_die)
		{	
			g_data->philo_dead = philo->name;
			break;
		}
	}
	return (NULL);
}

//-------------------------------------------------------------------------------
