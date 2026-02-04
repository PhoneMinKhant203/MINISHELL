/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:20:25 by wintoo            #+#    #+#             */
/*   Updated: 2025/09/01 12:01:36 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*dup;
	int		len;
	int		index;

	len = ft_strlen(str);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	index = 0;
	while (str[index])
	{
		dup[index] = str[index];
		index++;
	}
	dup[index] = '\0';
	return (dup);
}

// #include <string.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char	str[] = "I am lazy. I am sleepy. I hate working but want $";

// 	printf("%s\n", ft_strdup(str));
// 	printf("%s\n", strdup(str));
// }
