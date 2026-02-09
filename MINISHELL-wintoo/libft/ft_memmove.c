/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:35:57 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:21:45 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dist, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dist;
	s = src;
	if (!d && !s)
		return (NULL);
	if (d < s)
	{
		while (len--)
			*d++ = *s++;
	}
	else
	{
		d += len;
		s += len;
		while (len--)
			*--d = *--s;
	}
	return (dist);
}

// int	main(void)
// {
// 	int	src[10] = {1,2,3,4,5,6,7,8,9,10};
// 	int dest[10];

// 	ft_memmove(dest, src, sizeof(int)*10);
// 	for (int i = 0; i < 10; i++)
// 		printf("dest %d = %d\n", i, dest[i]);

// 	for (int i = 0; i < 10; i++)
// 		printf("source %d = %d\n", i, src[i]);

// 	ft_memmove(src+2, src, sizeof(int)*8);
// 	for (int i = 0; i < 10; i++)
// 		printf("dest %d = %d\n", i, dest[i]);

// 	for (int i = 0; i < 10; i++)
// 		printf("source %d = %d\n", i, src[i]);
// }

// int	main(void)
// {
// 	int	src[8] = {1,2,3,4,5,6};
// 	int dest[2] = {11, 12};

// 	// ft_memmove(dest, src, sizeof(int)*10);
// 	// for (int i = 0; i <= 5; i++)
// 	// 	printf("dest %d = %d\n", i, dest[i]);

// 	// for (int i = 0; i <= 5; i++)
// 	// 	printf("source %d = %d\n", i, src[i]);

// 	ft_memmove(src+2, src, sizeof(int)*6);
// 	for (int i = 0; i <= 5; i++)
// 		printf("dest %d = %d\n", i, dest[i]);

// 	for (int i = 0; i <= 7; i++)
// 		printf("source %d = %d\n", i, src[i]);
// }
