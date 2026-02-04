/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:34:57 by wintoo            #+#    #+#             */
/*   Updated: 2025/08/27 14:46:52 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	index;

	index = 0;
	while (str[index])
	{
		index++;
	}
	return (index);
}

// #include <string.h>

// int	main(void)
// {
// 	printf("%ld\n", ft_strlen("Hello 42 again, what chu doing."));
// 	printf("%ld\n", strlen("Hello 42 again, what chu doing."));
// }
