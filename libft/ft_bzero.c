/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:34:17 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/28 19:32:26 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	size_t			index;
	unsigned char	*s;

	s = (unsigned char *)str;
	index = 0;
	while (index < n)
		s[index++] = 0;
}

// #include <bsd/string.h>

// int	main(void)
// {
// 	char	s[] = "I am really sleepy right now.";

// 	bzero(s, 7);

// 	int	i = 0;
// 	while (i < sizeof(s))
// 	{
// 		printf("%c-", s[i]);
// 		i++;
// 	}
// 	printf("\n");

// 	char	s1[] = "I am really sleepy right now.";

// 	ft_bzero(s1, 7);

// 	i = 0;
// 	while (i < sizeof(s1))
// 	{
// 		printf("%c-", s1[i]);
// 		i++;
// 	}
// }
