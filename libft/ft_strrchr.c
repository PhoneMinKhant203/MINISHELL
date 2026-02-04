/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:55:08 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/28 21:53:39 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*ft_strrchr(const char *s, int c)
// {
// 	int		i;
// 	int		pos;

// 	i = 0;
// 	pos = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == (char)c)
// 			pos = i;
// 		i++;
// 	}
// 	if (s[pos] == (char)c)
// 		return ((char *)(s + pos));
// 	return (NULL);
// }

char	*ft_strrchr(const char *s, int c)
{
	char	*result;

	result = NULL;
	while (*s)
	{
		if (*s == (char)c)
			result = (char *)s;
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (result);
}

// int	main(void)
// {
// 	char	str2[] = "bonjour";

// 	printf("%s\n", ft_strrchr(str2, '\0'));
// 	printf("%s\n", strrchr(str2, '\0'));
// }
