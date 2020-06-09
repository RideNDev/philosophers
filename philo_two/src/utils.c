/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylegzoul <ylegzoul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 15:45:47 by ylegzoul          #+#    #+#             */
/*   Updated: 2020/06/09 15:45:49 by ylegzoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void			clean(void)
{
	int		i;

	i = 0;
	while (i < g_data->nb)
	{
		sem_unlink(ft_itoa(i));
		i++;
	}
	sem_unlink(FORK);
	sem_unlink(MSG);
	sem_unlink(END);
	free(g_data->philo);
}

int				get_time(void)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	return (((int)tmp.tv_sec * 1000) + ((int)tmp.tv_usec / 1000));
}

sem_t			*ft_sem(const char *str, int nb)
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
