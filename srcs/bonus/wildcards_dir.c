/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_dir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:46:29 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/15 00:47:16 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <dirent.h>

int	push_str(char ***arr, int *len, char *s)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (*len + 2));
	if (!new_arr)
		return (0);
	i = 0;
	while (i < *len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[*len] = s;
	new_arr[*len + 1] = NULL;
	if (*arr)
		free(*arr);
	*arr = new_arr;
	(*len)++;
	return (1);
}

static void	sort_strings(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i]) + 1) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	add_match(char ***matches, int *len, char *name)
{
	char	*dup;

	dup = ft_strdup(name);
	if (!dup)
		return (0);
	if (!push_str(matches, len, dup))
	{
		free(dup);
		return (0);
	}
	return (1);
}

char	**list_matches(const char *pattern, int *count)
{
	DIR				*d;
	struct dirent	*ent;
	char			**matches;
	int				len;

	*count = 0;
	matches = NULL;
	len = 0;
	d = opendir(".");
	if (!d)
		return (NULL);
	ent = readdir(d);
	while (ent)
	{
		if (match_pattern(pattern, ent->d_name))
		{
			if (!add_match(&matches, &len, ent->d_name))
				return (closedir(d), free2p(matches), NULL);
		}
		ent = readdir(d);
	}
	closedir(d);
	sort_strings(matches);
	return (*count = len, matches);
}
