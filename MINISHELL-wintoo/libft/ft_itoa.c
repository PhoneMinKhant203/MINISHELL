/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:53:53 by phonekha          #+#    #+#             */
/*   Updated: 2025/09/01 13:41:43 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nbr_len(long nb)
{
	int	i;

	i = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb = -nb;
		i++;
	}
	while (nb > 0)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*ret;
	long	ln;
	int		len;

	ln = n;
	len = ft_nbr_len(ln);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	if (ln == 0)
		ret[0] = '0';
	if (ln < 0)
	{
		ret[0] = '-';
		ln = -ln;
	}
	while (ln > 0)
	{
		len--;
		ret[len] = (ln % 10) + '0';
		ln /= 10;
	}
	return (ret);
}

// int	main(void)
// {
// 	int n = -1234567;

// 	char *res = ft_itoa(n);
// 	printf("%s",res);
// }
