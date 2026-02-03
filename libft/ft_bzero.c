/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:07:45 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:10:43 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *p, size_t number)
{
	size_t	i;

	i = 0;
	if (!p)
		return ;
	while (i < number)
	{
		*(unsigned char *)(p + i) = 0;
		i++;
	}
}

// void ft_bzero(void *p, size_t n)
// {
// 	ft_memset(p, 0, n);
// }
