/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:41:48 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:11:24 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t count)
{
	size_t	i;

	if (!str)
		return (NULL);
	i = 0;
	while (i < count)
	{
		*(unsigned char *)(str + i) = (unsigned char) c;
		i++;
	}
	return (str);
}

// int	main(void)
// {
// 	char str[15];

// 	ft_memset(str, 'a', sizeof(char) * 5);
// 	ft_memset(str + 5, 'b', sizeof(char) * 5);
// 	ft_memset(str + 10, 'c', sizeof(char) * 5);
// 	for (int i = 0; i < 15; i ++)
// 		printf("%c", str[i]);
// 	printf("\n");

// 	return (0);
// }
