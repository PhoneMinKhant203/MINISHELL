/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:07:28 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/28 17:31:22 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (i < size - 1 && *src)
		{
			dest[i++] = *src++;
		}
		dest[i] = '\0';
	}
	while (*src++)
		i++;
	return (i);
}

// #include <bsd/string.h>

// int	main(void)
// {
// 	char	dest[] = "agdsf safsd";
// 	char	src[] = "lorem ipsum dolor sit amet";

// 	printf("%ld\n", ft_strlcpy(dest, src, 0));
// 	printf("%ld\n", strlcpy(dest, src, 0));
// }
