#include "../inc/philosophers.h"

void			*ft_philo(void *tmp_philo)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo *)tmp_philo;
	if (pthread_create(&tid, NULL, &check_life, philo))
		return (NULL);
	while (1)
	{
		message(philo, 1);
		philo_eat(philo);
		if (philo->nb_of_eat >=
			g_data->number_of_time_each_philosophers_must_eat)
			sem_post(g_data->philo_ok);
		message(philo, 3);
		usleep(g_data->time_to_sleep * 1000);
	}
	return (NULL);
}

void			philo_eat(t_philo *philo)
{
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

void			*ft_nbeat(void *a)
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

void			init_philosophers(void)
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
