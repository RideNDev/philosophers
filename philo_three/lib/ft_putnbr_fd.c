/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylegzoul <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:23:36 by ylegzoul          #+#    #+#             */
/*   Updated: 2019/11/06 19:15:09 by ylegzoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void		ft_put(char c, int fd)
{
	write(fd, &c, 1);
}

void			ft_putnbr_fd(int nb, int fd)
{
	long int n;

	n = nb;
	if (n < 0)
	{
		n = -n;
		ft_put('-', fd);
	}
	if (n <= 9)
		ft_put(n + '0', fd);
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
}
