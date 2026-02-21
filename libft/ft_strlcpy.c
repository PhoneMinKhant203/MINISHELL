/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:51:33 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:27:40 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcpy(char *dist, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	if (len > 0)
	{
		while (src[i] != '\0' && i < len - 1)
		{
			dist[i] = src[i];
			i++;
		}
		dist[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}

// int main() {
//     char source[] = "Hello, World!";
//     char destination[10]; 
//     size_t result;

//     result = ft_strlcpy(destination, source, 5);

//     printf("Source: %s\n", source);
//     printf("Destination: %s\n", destination);
//     printf("Length of source string: %zu\n", result);

//     return 0;
// }
