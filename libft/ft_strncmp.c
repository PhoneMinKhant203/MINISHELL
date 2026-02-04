/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 14:59:27 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/28 16:55:59 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// #include <string.h>

// int	main()
// {
// 	char	s[] = "abcdefgh";
// 	char	s2[] = "";
// 	size_t	n = 0;

// 	printf("%d\n", ft_strncmp(s, s2, n));
// 	printf("%d\n", strncmp(s, s2, n));
// }
