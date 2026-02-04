/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:19:26 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/29 13:07:32 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dlen;
	size_t	slen;
	size_t	i;

	dlen = ft_strlen(dest);
	slen = ft_strlen(src);
	i = 0;
	if (dlen >= size)
		return (slen + size);
	while (src[i] && dlen + i < size - 1)
	{
		dest[dlen + i] = src[i];
		i++;
	}
	dest[dlen + i] = '\0';
	return (slen + dlen);
}

// #include <bsd/string.h>

// int	main(void)
// {
// 	char	*dest;
// 	dest = (char *)malloc(sizeof(*dest) * 15);
// 	dest[11] = 'a';
// 	char	src[] = "Hello Test Again";

// 	printf("%ld\n", ft_strlcat(dest, "lorem", 15));
// 	printf("%ld\n", strlcat(dest, "lorem", 15));
// }
