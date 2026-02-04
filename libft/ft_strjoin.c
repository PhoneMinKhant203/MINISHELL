/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:02:51 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/02 17:34:00 by wintoo           ###   ########.fr       */
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

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*res;
	int		s1len;
	int		s2len;
	int		i;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	res = malloc(s1len + s2len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
		res[s1len++] = s2[i++];
	res[s1len] = '\0';
	return (res);
}
