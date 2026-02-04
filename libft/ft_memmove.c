/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:28:58 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/29 14:44:25 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*des;
	char	*sr;

	if (dest == NULL && src == NULL)
		return (NULL);
	des = (char *)dest;
	sr = (char *)src;
	if (des > sr)
	{
		while (n-- > 0)
			des[n] = sr[n];
	}
	else
		ft_memcpy(dest, src, n);
	return (des);
}

// int	main(void)
// {
// 	// char	src[] = "hij";
// 	// char	des[] = "abcde";

// 	char	str[] = "123456";
// 	size_t	n = 3;

// 	printf("des - %s, src - %s\n", &str[0], &str[2]);
// 	printf("ft_memmove: %s\n\n", (char *) ft_memmove(&str[0], &str[2], n));

// 	char	str1[] = "123456";
// 	printf("des - %s, src - %s\n", &str1[0], &str1[2]);
// 	printf("memmove: %s\n\n", (char *) memmove(&str1[0], &str1[2], n));
// }
