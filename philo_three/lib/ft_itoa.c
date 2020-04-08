/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylegzoul <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 17:04:46 by ylegzoul          #+#    #+#             */
/*   Updated: 2019/11/05 17:04:51 by ylegzoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static char		*ft_rev(int i, char *tab)
{
	char	*s;
	int		j;

	j = 0;
	if (!(s = malloc(i + 2)))
		return (0);
	if (tab[0] == '-')
	{
		s[j] = '-';
		j++;
	}
	while (i >= 0 && tab[i] != '-')
	{
		s[j++] = tab[i--];
	}
	s[j] = 0;
	return (s);
}

char			*ft_itoa(int n)
{
	char			tab[12];
	int				i;
	unsigned int	k;

	i = 0;
	tab[0] = '0';
	if (n < 0)
	{
		k = -n;
		tab[0] = '-';
		i++;
	}
	else
		k = n;
	if (k == 0)
		i++;
	while (k != 0)
	{
		tab[i] = (char)(k % 10 + '0');
		i++;
		k = k / 10;
	}
	return (ft_rev(i - 1, tab));
}
