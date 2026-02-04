/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:21:08 by wintoo            #+#    #+#             */
/*   Updated: 2025/09/01 11:53:29 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *) &ptr[i]);
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	// char myStr[] = "Hello";

// 	// printf("%s\n", (char *)memchr(myStr, 'l', 1));

// 	char s[] = {0, 1, 2 ,3 ,4 ,5};

// 	printf("%s\n", (char *)ft_memchr(s, 6, 6));
// 	//printf("%s\n", s + 2);

// 	// int tab[7] = {-49, 49, 1, -1, 0, -2, 2};

// 	// printf("%s\n", (char *)ft_memchr(tab, -1, 7));
// }