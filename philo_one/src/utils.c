#include "../inc/philosophers.h"

void			message(t_philo *philo, int msg)
{
	int		time;

	pthread_mutex_lock(&g_data->msg);
	if (g_data->last_msg)
	{
		pthread_mutex_unlock(&g_data->msg);
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
	pthread_mutex_unlock(&g_data->msg);
}

int				get_time(void)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	return (((int)tmp.tv_sec * 1000) + ((int)tmp.tv_usec / 1000));
}

void			clean(void)
{
	int i;

	i = 0;
	while (i < g_data->nb)
	{
		pthread_mutex_destroy(&g_data->fork[i]);
		i++;
	}
	free(g_data->fork);
	pthread_mutex_destroy(&g_data->msg);
	pthread_mutex_destroy(&g_data->end);
	free(g_data->philo);
}
