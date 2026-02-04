/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:58:06 by wintoo            #+#    #+#             */
/*   Updated: 2025/09/01 16:04:09 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count(int n)
{
	int		count;
	long	nb;

	count = 0;
	nb = n;
	if (nb <= 0)
	{
		nb *= -1;
		count++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

char	*create_res(int nb)
{
	char	*res;
	int		count;

	count = ft_count(nb);
	res = malloc(count + 1);
	if (!res)
		return (NULL);
	res[count] = '\0';
	if (nb == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (nb < 0)
		res[0] = '-';
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	long	nb;
	int		count;

	count = ft_count(n) - 1;
	res = create_res(n);
	if (!res)
		return (NULL);
	nb = n;
	if (nb < 0)
		nb = -nb;
	while (nb > 0)
	{
		res[count] = (nb % 10) + '0';
		nb /= 10;
		count--;
	}
	return (res);
}

// int	main(void)
// {
// 	printf("%s\n", ft_itoa(0));
// }
