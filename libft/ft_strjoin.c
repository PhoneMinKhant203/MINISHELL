/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:24:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/03 13:18:53 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*mod_strjoin(const char *s1, const char *s2)
{
	char	*res;
	int		s1len;
	int		s2len;
	int		i;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	res = malloc(s1len + s2len + 2);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[s1len++] = '/';
	i = 0;
	while (s2[i])
		res[s1len++] = s2[i++];
	res[s1len] = '\0';
	return (res);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*res;

	res = (char *)malloc(sizeof(*s1) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		res[j] = s1[i];
		j++;
		i++;
	}
	i = 0;
	while (s2[i])
	{
		res[j] = s2[i];
		i++;
		j++;
	}
	res[j] = 0;
	return (res);
}

// int	main(void)
// {
// 	char *s1 = "Hello, World!";
// 	char *s2 = " My name is Phone Min Khant.";
// 	char *res = ft_strjoin(s1,s2);
// 	printf("%s", res);
// }
