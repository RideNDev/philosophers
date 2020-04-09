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
	sem_wait(g_data->end);
	sem_post(g_data->end);
	clean();	
	return (0);
}

//---------------------------- INIT_GAME -----------------------------------

int				init_game(int ac, char **av)
{
	g_data->nb = ft_atoi(av[1]);
	if (g_data->nb <= 1 || g_data->nb > 1500)
    {
        write(1, "nb of philo not aviable\n", 24);
        return (0);
    }
	g_data->time_to_die = ft_atoi(av[2]);
	g_data->time_to_eat = ft_atoi(av[3]);
	g_data->time_to_sleep = ft_atoi(av[4]);
	if (g_data->time_to_die < 0 || g_data->time_to_eat < 0 || g_data->time_to_sleep < 0)
    {
        write(1, "bad arguments\n", 14);
        return (0);
    }
	if (ac == 6)
		g_data->number_of_time_each_philosophers_must_eat = ft_atoi(av[5]);
	else
		g_data->number_of_time_each_philosophers_must_eat = 2147483647;
	if (g_data->number_of_time_each_philosophers_must_eat <= 0)
    {
        write(1, "bad arguments\n", 14);
        return (0);
    }
	if (!(g_data->philo = malloc(sizeof(t_philo) * g_data->nb)))
		return (0);
	g_data->fork = ft_sem(FORK, g_data->nb);
	g_data->msg = ft_sem(MSG, 1);
	g_data->end = ft_sem(END, 0);
	g_data->philo_ok = ft_sem(OK, 0);
	g_data->philo_dead = 0;
	g_data->philo_op = 0;
	g_data->last_msg = 0;
	init_philosophers();
	return (1);
}

void			init_philosophers()
{
	int		i;

	i = 0;
	while (i < g_data->nb)
	{
		g_data->philo[i].eat = ft_sem(ft_itoa(i), 1);
		g_data->philo[i].last_eat = 0;
		g_data->philo[i].name = i + 1;
		g_data->philo[i].nb_of_eat = 0;
		i++;
	}
}

void clean()
{
	int     i;

    i = 0;
    while (i < g_data->nb)
    {
		sem_unlink(ft_itoa(i));
		if(kill(g_data->philo[i].pid, 0) == 0)
            kill(g_data->philo[i].pid, SIGKILL);
		i++;
	}
	sem_unlink(FORK);
	sem_unlink(MSG);
	sem_unlink(END);
	i = 0;
	while (i < g_data->nb)
	{
		waitpid(g_data->philo[i].pid, NULL, 0);
		i++;
	}
	free(g_data->philo);
}

//----------------------------- START_GAME -----------------------------------

int				start_game()
{
	int			i;
	pthread_t	tid;

	i = 0;
	g_data->time_start = get_time();

	if (pthread_create(&tid, NULL, &ft_nbeat, NULL))
		return (0);
	while (i < g_data->nb)
	{
//		if (pthread_create(&tid, NULL, &ft_philo, &g_data->philo[i]))
//			return (0);
//		pthread_detach(tid);
		g_data->philo[i].last_eat = g_data->time_start;
		g_data->philo[i].pid = fork();
		if (g_data->philo[i].pid < 0)
			return (1);
		else if (g_data->philo[i].pid == 0)
		{
			ft_philo(&g_data->philo[i]);
			exit(0);
		}
		i++;
		usleep(100);
	}
	return (1);
}

int				end_game()
{
	int		time;

	if (g_data->philo_dead)
	{
		sem_wait(g_data->msg);
		g_data->last_msg = 1;
		time = get_time() - g_data->time_start;
		ft_putnbr_fd(time, 1);
		write(1, "\t", 1);
		ft_putnbr_fd(g_data->philo_dead, 1);
		write(1, " died\n", 6);
//		sem_post(g_data->msg);
		return (1);
	}
	else if (g_data->philo_op >= g_data->nb)
	{
		sem_wait(g_data->msg);
		g_data->last_msg = 1;
		write(1, "Everybody has eaten enough times.\n", 34);
//		sem_post(g_data->msg);
		return (1);
	}
	return (0);
}

int				get_time(void)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	return (((int)tmp.tv_sec * 1000) + ((int)tmp.tv_usec / 1000));
}

sem_t               *ft_sem(const char *str, int nb)
{	
 	sem_unlink(str);
    return (sem_open(str, O_CREAT | O_EXCL, 0644, nb));
}

void			message(t_philo *philo, int msg)
{
	int		time;

	sem_wait(g_data->msg);
	if (g_data->last_msg)
	{
		sem_post(g_data->msg);
		return ;
	}
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

void			*ft_philo(void *tmp_philo)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo *)tmp_philo;
	if (pthread_create(&tid, NULL, &check_life, philo))
		return(NULL);
//	pthread_detach(tid);
	while (1)
	{
		//------ THINK -----------------
		message(philo, 1);
		//------- EAT ------------------
		sem_wait(g_data->fork);
		message(philo, 4);
		sem_wait(g_data->fork);
		message(philo, 4);
		sem_wait(philo->eat);
		philo->last_eat = get_time();
		message(philo, 2);
		usleep(g_data->time_to_eat * 1000);
		sem_post(philo->eat);
		sem_post(g_data->fork);
		sem_post(g_data->fork);
		(philo->nb_of_eat)++;
		if (philo->nb_of_eat >= g_data->number_of_time_each_philosophers_must_eat)
			sem_post(g_data->philo_ok);
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
		sem_wait(philo->eat);
		if (get_time() - philo->last_eat >= g_data->time_to_die)
		{	
			g_data->philo_dead = philo->name;
			end_game();
			sem_post(philo->eat);
			sem_post(g_data->end);
			break ;
		}
		sem_post(philo->eat);
		usleep(1000);
	}
	return (NULL);
}

void			*ft_nbeat(void	*a)
{
	int		i;

	(void)a;
	i = 0;
	while (i < g_data->nb)
	{
		sem_wait(g_data->philo_ok);
		i++;
	}
	g_data->philo_op = g_data->nb + 1;
	end_game();
	sem_post(g_data->end);
	return (NULL);
}

//-------------------------------------------------------------------------------
