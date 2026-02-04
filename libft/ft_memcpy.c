/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:28:01 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/28 19:13:23 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*des;
	char	*sr;
	size_t	i;

	des = (char *)dest;
	sr = (char *)src;
	i = 0;
	while (i < n)
	{
		des[i] = sr[i];
		i++;
	}
	return (des);
}

// int	main(void)
// {
// 	// char	src[] = "hij";
// 	// char	des[] = "abcde";

// 	char	str[] = "zy\0xw\0vu\0\0tsr";
// 	size_t	n = 11;

// 	printf("des - %s, src - %s\n", &str[2], &str[0]);
// 	printf("ft_memcpy: %s\n\n", (char *) ft_memcpy("j", &str[0], n));

// 	char	str1[] = "abcdef";
// 	printf("des - %s, src - %s\n", &str1[2], &str1[0]);
// 	printf("memcpy: %s\n\n", (char *) memcpy("j", &str[0], n));
// }
