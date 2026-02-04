/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wintoo <wintoo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:46 by wintoo            #+#    #+#             */
/*   Updated: 2026/02/02 18:22:08 by wintoo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wordcount(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (!s[i + 1] || s[i + 1] == c))
			count++;
		i++;
	}
	return (count);
}

static	char	**ft_empty_arr(void)
{
	char	**arr;

	arr = malloc(sizeof(char *));
	arr[0] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		start;
	int		i;
	int		sub;

	if (!s)
		return (NULL);
	if (ft_strlen(s) == 0)
		return (ft_empty_arr());
	res = malloc(sizeof(char *) * (ft_wordcount(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	start = 0;
	sub = 0;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] && s[i + 1] != c)
			start = i + 1;
		if (s[i] != c && (!s[i + 1] || s[i + 1] == c))
			res[sub++] = ft_substr(s, start, i + 1 - start);
		i++;
	}
	res[sub] = NULL;
	return (res);
}
