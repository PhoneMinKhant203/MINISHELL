/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:51:30 by phonekha          #+#    #+#             */
/*   Updated: 2025/08/28 17:11:44 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_charcmp(char c1, char c2)
{
	if ((unsigned char)c1 != (unsigned char)c2)
		return ((unsigned char)c1 - (unsigned char)c2);
	return (0);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	while (str1[i] && str2[i] && i < n)
	{
		if (ft_charcmp(str1[i], str2[i]))
			return (str1[i] - str2[i]);
		i++;
	}
	if (i < n)
		return (ft_charcmp(str1[i], str2[i]));
	return (0);
}

// int	main(void)
// {
// 	const char *s1 = "Hello";
// 	const char *s2 = "Jello";

// 	int i = ft_strncmp(s1, s2, 4);
// 	printf("%d", i);

// 	return (0);
// }
