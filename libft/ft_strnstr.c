/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:14:09 by phonekha          #+#    #+#             */
/*   Updated: 2025/09/02 14:45:04 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!s2 || !s2[0])
		return ((char *)s1);
	if (!s1 || !s2)
		return (NULL);
	while (i < n && s1[i])
	{
		j = 0;
		while (s1[i + j] && s2[j] && i + j < n && s1[i + j] == s2[j])
			j++;
		if (!s2[j])
			return ((char *)(s1 + i));
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char s1[] = "This is the way.";
// 	char s2[] = "the";

// 	char *res = ft_strnstr(s1, s2, 10);
// 	printf("The result: %s\n", res);

// }
