/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:41:03 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:11:00 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int chr, size_t n)
{
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	while (i < n)
	{
		if (*(unsigned char *)(str + i) == (unsigned char)chr)
			return ((void *)(str + i));
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char data[] = {'q','r','s','t','p','a','x'};

// 	char *res = ft_memchr(data, 't', 7);

// 	printf("res[0] = %c\n", res[0]);
// 	printf("res[1] = %c\n", res[1]);

// 	return (0);
// }
