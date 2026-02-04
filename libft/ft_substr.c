/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:00:48 by wintoo            #+#    #+#             */
/*   Updated: 2025/09/01 14:36:02 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	l;
	size_t	index;

	if (s == NULL)
		return (NULL);
	l = ft_strlen(s);
	if (start > l)
		return (ft_strdup(""));
	if (len > l - start)
		len = l - start;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	index = 0;
	while (s[start] && index < len)
		res[index++] = s[start++];
	res[index] = '\0';
	return (res);
}

// int	main(void)
// {
// 	char	str[] = "lorem ipsum dolor sit amet";

// 	printf("%s\n", ft_substr(str, 400, 20));
// }
