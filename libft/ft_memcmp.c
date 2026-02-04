/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:18:02 by wintoo            #+#    #+#             */
/*   Updated: 2025/09/01 11:41:46 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (str1[i] == str2[i] && i < n - 1)
		i++;
	return (str1[i] - str2[i]);
}

// int	main(void)
// {
// 	// char myStr1[] = "ABC EFH";
// 	// char myStr2[] = "ABC EF^";

// 	// printf("%d\n", memcmp(myStr1, myStr2, 7));
// 	// printf("%d\n", ft_memcmp(myStr1, myStr2, 7));

// 	char s2[] = {0, 0, 127, 0};
// 	char s3[] = {0, 0, 42, 0};
// 	printf("%d\n", ft_memcmp(s2, s3, 4));
// }
