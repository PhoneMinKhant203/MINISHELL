/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:13:09 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:11:06 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dist, const void *src, size_t number)
{
	size_t	i;

	i = 0;
	if (!dist)
		return (NULL);
	while (i < number)
	{
		*(char *)(dist + i) = *(char *)(src + i);
		i++;
	}
	return (dist);
}

// int	main(void)
// {
// 	char s[] = "abcdefghijklmnopqrstuvwxyz";
// 	char d[27];

// 	ft_memcpy(d, s, sizeof(char) * 27);
// 	printf("d: %s\n", d);

// 	char d2[10];
// 	ft_memcpy(d2, s+8, sizeof(char) * 9);
// 	d2[9] = '\0';

// 	printf("d2: %s\n", d2);
// }

// int	main(void)
// {
// 	int	src[6] = {1,2,3,4,5,6};
// 	int dest[2] = {11, 12};

// 	// ft_memmove(dest, src, sizeof(int)*10);
// 	// for (int i = 0; i <= 5; i++)
// 	// 	printf("dest %d = %d\n", i, dest[i]);

// 	// for (int i = 0; i <= 5; i++)
// 	// 	printf("source %d = %d\n", i, src[i]);

// 	ft_memcpy(src+3, src, sizeof(int)*6);
// 	for (int i = 0; i <= 5; i++)
// 		printf("dest %d = %d\n", i, dest[i]);

// 	for (int i = 0; i <= 5; i++)
// 		printf("source %d = %d\n", i, src[i]);
// }
